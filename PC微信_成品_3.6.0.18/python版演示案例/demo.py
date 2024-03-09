import subprocess
from http.server import HTTPServer, BaseHTTPRequestHandler
from threading import Thread
import requests
import json
import time
import winreg
import sys


def get_wx_version():
    """获取微信版本号"""

    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, r"Software\Tencent\WeChat", 0, winreg.KEY_READ) as key:
            int_version = winreg.QueryValueEx(key, "Version")[0]
            hex_version = hex(int_version)
            hex_str = hex_version[2:]
            new_hex_str = "0" + hex_str[1:]
            new_hex_num = int(new_hex_str, 16)
            major = (new_hex_num >> 24) & 0xFF
            minor = (new_hex_num >> 16) & 0xFF
            patch = (new_hex_num >> 8) & 0xFF
            build = (new_hex_num >> 0) & 0xFF
            return "{}.{}.{}.{}".format(major, minor, patch, build)
    except Exception as e:
        print("打开注册表失败：{}".format(e))
        return None


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

    # 校验微信版本号是否正确
    wx_version = get_wx_version()
    if wx_version != "3.6.0.18":
        print("当前微信版本为：【{}】，请确保你的微信版本是【3.6.0.18】".format(wx_version))
        sys.exit(0)

    dll_port = 8989  # dll的端口号
    my_port = 9000   # 自己的端口号

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

