
from dataclasses import dataclass
import requests

@dataclass
class InputTest:
    req_urls: list[str]
    req_body: str
    req_headers: dict[str, str]

def test_httpp():
    test = InputTest(
            req_urls=["http://localhost:8080/proof", "http://localhost:8080/proof-again"],
            req_body="Ping",
            req_headers= {
                "Content-Type": "text/plain",
            }
    )

    for url in test.req_urls:
        response = requests.post(url, data=test.req_body, headers=test.req_headers)
        assert response.status_code == 200
        assert response.text == "Pong"

if __name__ == "__main__":
    test_httpp()
