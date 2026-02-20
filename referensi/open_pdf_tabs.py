"""
Buka semua PDF link dari url_list.txt di Chrome tab baru
Jeda 2 detik tiap tab
"""
import subprocess
import time
import os

CHROME_PATH = r"C:\Program Files\Google\Chrome\Application\chrome.exe"
URL_FILE = os.path.join(os.path.dirname(__file__), "url_list.txt")

if not os.path.exists(CHROME_PATH):
    # Coba path alternatif
    CHROME_PATH = r"C:\Program Files (x86)\Google\Chrome\Application\chrome.exe"

with open(URL_FILE, encoding="utf-8") as f:
    urls = [line.strip() for line in f if line.strip()]

print(f"Total URL: {len(urls)}")
print(f"Chrome: {CHROME_PATH}")
print("Mulai buka tab... (2 detik per tab)\n")

for i, url in enumerate(urls, 1):
    print(f"[{i:03d}/{len(urls)}] {url[:90]}")
    try:
        subprocess.Popen([CHROME_PATH, "--new-tab", url])
    except Exception as e:
        print(f"  ERROR: {e}")
    time.sleep(2)

print("\nSelesai! Semua URL telah dibuka.")
