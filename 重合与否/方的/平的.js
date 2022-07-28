const [x1,y1] = [0,0]
const [x2,y2] = [1,1]

const [u1,v1] = [1,0.9]
const [m1,n1] = [2,2]
const [m2,n2] = [3,3]

function 比线点(a,b,i){
    // console.log(a,b,i);
    [a,b] = a >= b ? [a,b] : [b,a];
    return a>i && i>b;
}

function 比两线重叠(x1,x2,m1,m2){
    // console.log(x1,x2,m1,m2);
    return 比线点(x1,x2,m1) || 比线点(x1,x2,m2);
}

function 比两面相交(x1,y1,x2,y2,m1,n1,m2,n2){
    return 比两线重叠(x1,x2,m1,m2) && 比两线重叠(y1,y2,n1,n2);
}

// console.log(比两面相交(x1,y1,x2,y2,m1,n1,m2,n2))
// console.log(比两面相交(x1,y1,x2,y2,u1,v1,m2,n2))



function 比线点2(a,b,i){
    // console.log(a,b,i);
    // [a,b] = a >= b ? [a,b] : [b,a];
    return a==i || i==b;
}

function 比两线重叠2(x1,x2,m1,m2){
    // console.log(x1,x2,m1,m2);
    return 比线点2(x1,x2,m1) || 比线点2(x1,x2,m2);
}

function 比两面相切(x1,y1,x2,y2,m1,n1,m2,n2){
    return 比两线重叠2(x1,x2,m1,m2) || 比两线重叠2(y1,y2,n1,n2);
}

// console.log(比四线重叠(x1,y1,x2,y2,m1,n1,m2,n2))
console.log(比两面相切(x1,y1,x2,y2,u1,v1,m2,n2))

