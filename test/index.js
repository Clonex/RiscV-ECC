import SerialPort from "serial-node";
const serialList = new SerialPort().list();

async function start()
{
    serialList.forEach(d => console.log("Port", d)); 
}

start();