import sys
import json

if __name__ == '__main__':
    source = sys.argv[1]
    dest = sys.argv[2]

    data = None
    with open(source, "r") as f:
        data = json.load(f)
    with open(dest, "w") as f:
        f.write("File: Webring\n\n")
        for entry in data:
            f.write(f"- <{entry['name']} at {entry['protocols']['http']['clearnet']}> : {entry['description']}\n\n")
