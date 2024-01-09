import subprocess
from http.server import HTTPServer, BaseHTTPRequestHandler
from threading import Thread
import requests
import json
import time


class HttpServer(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path == "/msg":
            # 获取数据
            json_str = self.rfile.read(int(self.headers["content-length"]))
            json_str = json_str.decode()
            result = json.loads(json_str)
            print(result)

            self.send_response(200)
            self.send_header("Content-type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps({"result": "ok"}).encode())


if __name__ == '__main__':
    # 注意！！！请确保你的微信版本是【3.6.0.18】
    # 注意！！！请确保你的微信版本是【3.6.0.18】
    # 注意！！！请确保你的微信版本是【3.6.0.18】

    dll_port = 8748  # dll的端口号
    my_port = 9001   # 自己的端口号

    # 1、启动http服务【用来监听消息】
    Thread(target=lambda: HTTPServer(("127.0.0.1", my_port), HttpServer).serve_forever()).start()

    # 2、调用命令行启动微信
    ret = subprocess.Popen(
        r'"{}" start {} --my_port={}'.format("../inject_tool.exe", dll_port, my_port),
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    output, error = ret.communicate()

    # 3、循环等待登录
    while True:
        time.sleep(1)
        # 获取登录状态
        response = requests.post("http://127.0.0.1:{}/api".format(dll_port), json={"type": 1})
        ret = json.loads(response.content.decode())
        is_login = ret["data"]["status"]
        if is_login:
            break

    # 4、登录成功，获取个人信息
    response = requests.post("http://127.0.0.1:{}/api".format(dll_port), json={"type": 3})
    ret = json.loads(response.content.decode())
    wx_info = ret["data"]
    print(wx_info)

    # 5、发送文本消息
    requests.post("http://127.0.0.1:{}/api".format(dll_port), json={
        "type": 7,
        "wx_id": "filehelper",
        "msg": "我是文本消息"
    })

    # ...其他操作

    # 阻塞防止程序退出...
    while True:
        time.sleep(1)

