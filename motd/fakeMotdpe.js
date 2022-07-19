import dgram from 'node:dgram';
import fs from 'fs';



const server = dgram.createSocket('udp4');

function hexToBytes(hex) {
    hex = hex.replaceAll(' ','');
    for (var bytes = [], c = 0; c < hex.length; c += 2)
    bytes.push(parseInt(hex.substr(c, 2), 16));
    return bytes;
    // Function Powered By https://github.com/asktaosec
}

function stringToHex(str) {
    return str.split("").map(function(c) {
        return ("0" + c.charCodeAt(0).toString(16)).slice(-2);
    }).join("");
}



const randomHex = () => Math.floor(Math.random()*0xffff).toString(16).padEnd(4, "a");
// hexToBytes('0100000000'+ randomHex() +'c1 1d 00 ff ff 00 fe fe fe fe fd fd fd fd 12 34 56 78 9c 18 28 7f e1 64 89 8d')

const keyHand = ['0100000000', randomHex() ,'c11d00ffff00fefefefefdfdfdfd123456789c18287fe164898d']
const keyHandLength = keyHand.join('').length;

let motdpeHand = ['1c00000000',"6d2a",'c11d806c7ed9eec97c1f00ffff00fefefefefdfdfdfd12345678005d'];
const motdpeHandLength = keyHand.join('').length;

const motdpeArray = [
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
// console.log
fs.writeFileSync('p',Buffer.from(hexToBytes(motdpeHand.join('')).concat(hexToBytes(stringToHex(motdpeArray.join(";"))))))
server.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});

server.on('message', (msg, rinfo) => {
    console.log(msg)
    msg = msg.toString('hex'); 
    if(!msg.startsWith(keyHand[0]) || msg.length !== keyHandLength){
       console.log(" error => ",msg)
       
        // return
    }

    // console.log(msg.slice(10,14), `${rinfo.address}:${rinfo.port}`)
        motdpeHand[1] = msg.slice(10,14);
    server.send(Buffer.from(hexToBytes(motdpeHand.join('')).concat(hexToBytes(stringToHex(motdpeArray.join(";"))))), rinfo.port, rinfo.address, (err) => {});
    console.log(""+Buffer.from(hexToBytes(motdpeHand.join('')).concat(hexToBytes(stringToHex(motdpeArray.join(";"))))))

});

server.on('listening', () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
});


server.bind(41234);
