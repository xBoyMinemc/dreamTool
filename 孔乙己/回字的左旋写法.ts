const xBoyDream : Map<number,Array<number>> = new Map();
const toNegative = (i:number) : number => i < 0 ?  i : -i;
const toPositive = (i:number) : number => i < 0 ? -i :  i;
// let  x : number = 0,y : number = 0;


// const xy = [-1,3]
const pos2index = (xy : Array<number>) : number=>{

    let x : number = xy[0], y : number = xy[1];

    if (x === -y && x <= 0) return (x*2 -1) ** 2 -1;

    let inner : number = 1+toNegative(toPositive(x) > toPositive(y) ? x : y);
    // console.log("inner=>",inner)
    let inBas : number = (inner*2 -1) ** 2;
    // console.log("inBas=>",inBas)

    if (x=== inner-1)return 1+inBas+toPositive(-inner-y)                              -1
    if (y=== inner-1)return 1+inBas+toPositive( inner-x)  +    toPositive((inner-1)*2)-1
    if (x===-inner+1)return 1+inBas+toPositive( inner-y)  +  2*toPositive((inner-1)*2)-1
    if (y===-inner+1)return 1+inBas+toPositive(-inner-x)  +  3*toPositive((inner-1)*2)-1

}







const index2pos = ( location : number) : Array<number> =>{
    if (!location) return [0,0]

    let inner : number = -(Math.sqrt(location+1)/2-1).toFixed(0);
    let side = -(inner*2)+3
    let inBas : number = (side-2) ** 2;
    
    if (inBas === location+1)return [(inner),-(inner)];

    let onBas : number = location - inBas + 1 ;

    if (onBas<=side*1-1) return [inner-1,-inner+1-onBas]
    if (onBas<=side*2-2) return [inner+onBas-(side)-1+1,inner-1]
    if (onBas<=side*3-3) return [-inner+1,inner+onBas-((side)*2-2)-1]
                         return [-inner-(onBas-((side)*3-3))+1,-inner+1]

}

interface kyj  {
    pos2index : number,
    index2pos : Array<number>
}

const kyj  = {
    pos2index : pos2index,
    index2pos : index2pos
}
let x = -9,y = -9
// export default kyj;
for(let x = -9;x<=9;x++)
for(let y = -9;y<=9;y++)
// console.log(x,y,pos2index([x,y]))
if (!([x,y].toString()==index2pos(kyj.pos2index([x,y])).toString()))
console.log(x,y,kyj.pos2index([x,y]),index2pos(kyj.pos2index([x,y])),[x,y].toString()==index2pos(kyj.pos2index([x,y])).toString())


