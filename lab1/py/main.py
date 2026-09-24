from pathlib import Path
import numpy as np

base = Path(__file__).resolve().parent.parent / "data"

sizes = [200, 400, 800, 1200, 1600, 2000]

for n in sizes:
    try:
        a = np.loadtxt(base / f"m{n}_1.txt", dtype=np.int64).reshape(n, n)
        b = np.loadtxt(base / f"m{n}_2.txt", dtype=np.int64).reshape(n, n)
        c = np.loadtxt(base / "result" / f"result{n}.txt", dtype=np.int64, comments='#')

        print(f"[{n}x{n}] {'OK' if np.array_equal(a @ b, c) else 'FAIL'}")
    except FileNotFoundError as e:
        print(f"[{n}x{n}] skip ({e.filename})")