import requests
import argparse
from urllib.parse import urljoin

WEBRING_DEFURL = "https://webring.devse.wiki"

def get_webring_entries(url):
    json_url = urljoin(url, "/webring.json")
    req = requests.get(json_url)
    if req.status_code != requests.codes.ok:
        return []
    return req.json()

def main(args):
    entries = get_webring_entries(args.url)

    with open(args.output, "w") as f:
        f.write("File: Webring\n\n")
        for entry in entries:
            f.write(f"- <{entry['name']} at {entry['protocols']['http']['clearnet']}> : {entry['description']}\n\n")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='devsewebring.py')
    parser.add_argument('-u', '--url',
                        type=str, help='set webring url',
                        default=WEBRING_DEFURL)
    parser.add_argument('-o', '--output', type=str, help='output file')

    args = parser.parse_args()

    main(args)
