import SerialPort from "serialport";
import { dataParser, MODES } from "./helpers.js";

let targetPort = process.argv[2];
if(!targetPort)
{
    console.log("No port defined, default used. (2)");
    targetPort = 2;
}

const port = new SerialPort('/dev/pts/' + targetPort, {
    baudRate: 115200
});

function write(message)
{
    return new Promise(r => port.write(message, r));
}

port.on('open', async () => {
    console.log("Opened connection..");
    await write([MODES.PING]);
});

let karaData = [];
let karaI = false;

port.on('data', async (data) => {
    let message = dataParser(data);
    if(message)
    {
        if(typeof karaI === "number" && (message.ping || message.checksum))
        {
            if(message.checksum)
            {
                karaData[karaI] = message.cycles;
                if(karaI >= 64)
                {
                    console.log("Got results", karaData);
                    return;
                }
                console.log("Got results for", karaI, message);
            }else{
                console.log("Starting..");
            }
            await write([MODES.SET_KARAT]);
            await write([karaI++]);

            await write([MODES.SEED]);
            for(let i = 0; i < 48; i++)
            {
                await write([7]);
            }

            await write([MODES.HASH]);
        }else if(message.ping)
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