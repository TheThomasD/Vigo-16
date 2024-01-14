Import("env")
import gzip
import shutil

print("---> extra_script.py <---")
print("Current Build targets:", BUILD_TARGETS)

if 'buildfs' in BUILD_TARGETS:
    files = ['index.html', 'script.js']
    for file in files:
        print("Zipping " + file + "...")
        with open('data-unzipped/' + file, 'rb') as file_in:
            with gzip.open('data/' + file + '.gz', 'wb') as file_out:
                shutil.copyfileobj(file_in, file_out)