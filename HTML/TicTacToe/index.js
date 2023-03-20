const MIN_SIZE = 3;
const MAX_SIZE = 9;
const size = document.getElementById("size-txt");
const players = [document.getElementById('pts-1'), document.getElementById('pts-2')]
const chars = [' ','X','O'];
const SIZE = 3;

// Initialized Containers
const crowContainer = new Array(3).fill(null).map(()=>{
    return new Array(2).fill(0);
})
const ccolContainer = new Array(3).fill(null).map(()=>{
    return new Array(2).fill(0);
})
const cdiagContainer = new Array(2).fill(null).map(()=>{
    return new Array(2).fill(0);
})

// container for runtime
let rowContainer = crowContainer.map((i)=>{ return i.slice();});
let colContainer = ccolContainer.map((i)=>{ return i.slice();});
let diagContainer = cdiagContainer.map((i)=>{ return i.slice();});


let table = [
    [0,0,0],
    [0,0,0],
    [0,0,0],
];
let turn = 1;


// function increment_size(){
//     let val = Number(size.innerHTML) + 1;
//     if (val > MAX_SIZE) val = MAX_SIZE;
//     size.innerHTML = val;
//     generate_table(val);
// }
// function decrement_size(){
//     let val = Number(size.innerHTML) - 1;
//     if (val < MIN_SIZE) val = MIN_SIZE;
//     size.innerHTML = val;
//     generate_table(val);
// }

const corner = {
    0: {
        0: 'top-left',
        2: 'top-right',
    },
    2: {
        0: 'bot-left',
        2: 'bot-right',
    },
};
let tableContainer = document.getElementsByClassName('container-table');
function generate_table(){
    // console.log(tableContainer);
    for (let i=0;i<table.length;i++){
        for(let j=0;j<table.length;j++){
            let newCell = document.createElement('button');
            newCell.classList.add('items');
            newCell.setAttribute('OnClick', `move(this,${i},${j})`)
            newCell.innerHTML = chars[0];
            if ((i == 0 || i == table.length-1) && (j == 0 || j == table.length-1)){
                newCell.classList.add(corner[i][j]);
            }
            tableContainer[0].appendChild(newCell);
        }
    }
}

function move(e,row,col){
    if (table[row][col] != 0) {
        console.log(table);
        alert("Invalid Move.");
        return;
    }
    e.innerHTML = chars[turn];
    if (turn <= 0) { turn = 1;}
    table[row][col] = turn;
    turn ++;
    if (turn > 2 ){
        turn = 1;
    }
    // console.log(table);
    
    ctrPts();
    
    // console.log(`row ${rowContainer}`);
    // console.log(`col ${colContainer}`);
    // console.log(`diag ${diagContainer}`);
    
    let x = evalWinner();
    console.log(x);
    if (x != 0) { 
        if (x == -1){
            alert("Draw");
        }
        else{
            alert(`Player ${x} Wins`);
            players[x-1].innerHTML = Number(players[x-1].textContent) + 1;
        }
        reset();
    }
}

function ctrPts(){
    for(let i = 0;i<table.length;i++){
        for(let j = 0;j<table.length;j++){
            let player = table[i][j] - 1;
            if (player <= -1 || player >= 2) { continue; }
            //  Row
            rowContainer[i][player] += 1;
            // Col
            colContainer[j][player] += 1;
        }
    }

    let sDiag = [0,0];
    let bDiag = [0,table.length-1];
    for (let i = 0;i<table.length;i++){
        sPlayer = table[sDiag[0]][sDiag[1]] - 1;
        if (sPlayer >=0 && sPlayer <2) { 
            diagContainer[0][sPlayer] += 1;
            sDiag[0] += 1;
            sDiag[1] += 1;
        }
        
        bPlayer = table[bDiag[0]][bDiag[1]] - 1;
        if (bPlayer >=0 && bPlayer <2) { 
            diagContainer[1][bPlayer] += 1;
            bDiag[0] += 1;
            bDiag[1] -= 1;
        }
    }
}

function evalWinner(){
    for (let i =0;i<table.length;i++){
        for(let j=0;j<2;j++){
            if (rowContainer[i][j] == table.length || colContainer[i][j] == table.length) { return j + 1;}
            if (i >= 2){continue;}
            if (diagContainer[i][j] == table.length) { return j + 1; }
        }
    }
    if ( !table.find(x => { return x.includes(0);} ) ){
        return -1;
    }
    rowContainer = crowContainer.map((i)=>{return i.slice();});
    colContainer = ccolContainer.map((i)=>{return i.slice();});
    diagContainer = cdiagContainer.map((i)=>{return i.slice();});
    return 0;
}

function reset(){
    turn = 1;
    table = table.map(()=> {return [0,0,0]});
    rowContainer = crowContainer.map((i)=>{ return i.slice();});
    colContainer = ccolContainer.map((i)=>{ return i.slice();});
    diagContainer = cdiagContainer.map((i)=>{ return i.slice();});
    let x = document.getElementsByClassName('items');
    for (let i=0;i<table.length * table.length;i++){
        x[i].innerHTML = chars[0];
    }
}

generate_table();