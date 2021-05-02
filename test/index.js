// import SerialPort from "serial-node";
// const serialList = new SerialPort().list();

// async function start()
// {
//     serialList.forEach(d => console.log("Port", d)); 
// }

// start();

import SerialPort from "serialport";
import {dataParser} from "./helpers.js";
const port = new SerialPort('/dev/pts/2', {
    baudRate: 115200
});

const MODES = {
    HASH: 1,
    SEED: 2,
    PING: 3,
};

function write(message)
{
    return new Promise(r => port.write(message, r));
}

port.on('open', async () => {
    console.log("Opened connection..");
    await write([MODES.PING]);
});

port.on('data', async (data) => {
    let message = dataParser(data);
    if(message)
    {
        if(message.ping)
        {
            console.log("Setting seed..");
            await write([MODES.SEED]);
            for(let i = 0; i < 48; i++)
            {
                await write([7]);
            }

            console.log("Starting hash..");
            await write([MODES.HASH]);
        }else if(message.checksum)
        {
            console.log("Got hash", message);
            await write([MODES.HASH]);
            console.log("Starting hash..");
        }else{
            console.log("Got something", message);
        }
    }
});

port.on('error', (err) => {
  console.log('Error: ', err.message);
});