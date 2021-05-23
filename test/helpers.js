const disallowed = [32, 10];
let temp = "";
let lastChar = false;
export function dataParser(data)
{
    let done = false;
    for(let i = 0; i < data.length; i++)
    {
        const d = data[i];
        if(d === 44 && lastChar === 125)
        {
            done = true;
        }else if(!disallowed.includes(d))
        {
            temp = temp + String.fromCharCode(d);
        }
        lastChar = d;
    }

    if(done)
    {
        let msg = temp;
        temp = "";
        lastChar = false;
        return JSON.parse(msg);
    }

    return false;
}

export const MODES = {
    HASH: 1,
    SEED: 2,
    PING: 3,
    SET_KARAT: 4,
};