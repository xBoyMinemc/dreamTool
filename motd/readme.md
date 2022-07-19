import Getmotdpe from "motdpe.js"

const motdpe = Getmotdpe("127.0.0.1",19132)

#

返回一个数组

[   
    "MCPE",//服务器类型  
    "IC inner - O - Powered By ...",//名称  
    "527",//协议版本  
    "0.8.9",//显示的版本号  
    "43",//在线玩家  
    "2022",//最大玩家数量  
    "9253910808818711583",//内部版本号  
    "Test For IC mod in PE",//存档名  
    "S",//默认游戏模式名  
    "1",//默认游戏模式序号  
    "41234",//ipv4 port  
    "6661",//ipv6 port  
    "" //占位，留空  
]  
#
伪造一个务器发送motdpe
#

import  "fakeMotdpe.js";

将会在计算机指定端口模拟一个能发送motdpe的基岩服
