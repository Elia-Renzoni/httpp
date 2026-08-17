
from dataclasses import dataclass
import requests

@dataclass
class Test:
    req_urls: list[str]
    req_body: str
    req_headers: dict[str, str]
    exp_status: list[int]
    exp_res: list[str]


def test_httpp():
    test = Test(
            req_urls=["http://localhost:8080/proof", "http://localhost:8080/proof-again", "http://localhost:8080/fake-endpoint"],
            req_body="Ping",
            req_headers= {
                "Content-Type": "text/plain",
            },
            exp_status=[200, 200, 400],
            exp_res=["Pong", "Pong", "function handler not found for fake-endpoint"],
    )

    for test_case in range(3):
        url = test.req_urls[test_case]
        body = test.req_body
        hd = test.req_headers
        status_code = test.exp_status[test_case]
        text = test.exp_res[test_case]

        response = requests.post(url, data=body, headers=hd)

        assert response.status_code == status_code
        assert response.text == text

if __name__ == "__main__":
    test_httpp()
