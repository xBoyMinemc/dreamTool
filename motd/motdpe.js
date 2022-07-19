import dgram from 'node:dgram';

const GetMotdpe = (ip,port) => {
    const server = dgram.createSocket('udp4');


    function hexToBytes(hex) {
        hex = hex.replaceAll(' ', '');
        for (var bytes = [], c = 0; c < hex.length; c += 2)
            bytes.push(parseInt(hex.substr(c, 2), 16));
        return bytes;
        // Function Powered By https://github.com/asktaosec
    }

    server.on('error', (err) => {
        console.log(`server error:\n${err.stack}`);
        server.close();
    });

    server.on('message', (msg, rinfo) => {
        const motdpe = String(msg).slice(35).split(';')
        console.log(motdpe.join('\n'));

        server.close();
        return motdpe;
    });

    server.on('listening', () => {
        const address = server.address();
        console.log(`server listening ${address.address}:${address.port}`);
    });




    const randomHex = () => {
        const g = Math.floor(Math.random() * 0xffff).toString(16).padEnd(4, "0");
        console.log(g)
        return g
    }
    const message = hexToBytes('01 00 00 00 00' + randomHex() + 'c1 1d 00 ff ff 00 fe fe fe fe fd fd fd fd 12 34 56 78 9c 18 28 7f e1 64 89 8d')

    server.bind(41234);
    server.send(Buffer.from(message), port, ip, (err) => { });
}

export default GetMotdpe;
