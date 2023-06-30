# 获取基岩版服务器motd

## C
傻瓜式使用方法，更适合中国宝宝体制
```sh
motdpe
motdpe ip
motdpe ip:port
motdpe ip port
```
```text
> motdpe play.easecation.net

O 服务器类型: MCPE
O 服务器名称: EaseCation EC BED WARS EC
O 协议=>140
O 版本=>1.18.0
O 在线人数: 1
O 最大人数: 500
O 地图名=> Powered by Nemisys
O 默认模式: Survival
O 默认模式: 1
O 解析地址: 173.208.172.170

```

## JavaScript

```JS
import Getmotdpe from "motdpe.js"

const motdpe = Getmotdpe("127.0.0.1",19132)

//返回值
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
```
# 伪造一个基岩版服务器发送motd

将会在计算机指定端口模拟一个能发送motd的基岩服
```JS
import  "fakeMotdpe.js";
```
 
