"""Test concurrent access to ObjC type caches."""

import threading
from concurrent.futures import ThreadPoolExecutor

from objctypes import ObjCClass, ObjCSelector


def test_objcselector_concurrent_from_name() -> None:
    """Test concurrent ObjCSelector cache access via from_name."""
    results: list[ObjCSelector] = []
    lock = threading.Lock()

    def worker() -> None:
        for _ in range(100):
            sel = ObjCSelector("someMethod")
            with lock:
                results.append(sel)

    with ThreadPoolExecutor(max_workers=10) as executor:
        futures = [executor.submit(worker) for _ in range(10)]
        for future in futures:
            future.result()

    first = results[0]
    assert all(s is first for s in results)


def test_objcselector_concurrent_from_address() -> None:
    """Test concurrent ObjCSelector cache access via from_address."""
    sel = ObjCSelector("someMethod")
    address = sel.address
    results: list[ObjCSelector] = []
    lock = threading.Lock()

    def worker() -> None:
        for _ in range(100):
            s = ObjCSelector.from_address(address)
            with lock:
                results.append(s)

    with ThreadPoolExecutor(max_workers=10) as executor:
        futures = [executor.submit(worker) for _ in range(10)]
        for future in futures:
            future.result()

    assert all(s is sel for s in results)


def test_objcclass_concurrent_from_name() -> None:
    """Test concurrent ObjCClass cache access via from_name."""
    results: list[type] = []
    lock = threading.Lock()

    def worker() -> None:
        for _ in range(100):
            cls = ObjCClass.from_name("NSObject")
            with lock:
                results.append(cls)

    with ThreadPoolExecutor(max_workers=10) as executor:
        futures = [executor.submit(worker) for _ in range(10)]
        for future in futures:
            future.result()

    first = results[0]
    assert all(c is first for c in results)


def test_objcclass_concurrent_from_address() -> None:
    """Test concurrent ObjCClass cache access via from_address."""
    cls = ObjCClass.from_name("NSObject")
    address = cls.address
    results: list[type] = []
    lock = threading.Lock()

    def worker() -> None:
        for _ in range(100):
            c = ObjCClass.from_address(address)
            with lock:
                results.append(c)

    with ThreadPoolExecutor(max_workers=10) as executor:
        futures = [executor.submit(worker) for _ in range(10)]
        for future in futures:
            future.result()

    assert all(c is cls for c in results)
