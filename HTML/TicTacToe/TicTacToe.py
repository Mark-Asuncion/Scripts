# -- TO DO --
# Fix Bug where container doesn't count if the table is not complete(no 0)


tbl_char = [' ','X','0']

tbl = [
    [0,0,2],
    [1,2,1],
    [2,1,1],
]
# tbl = [
#     [1,2,1],
#     [2,1,2],
#     [2,2,2],
# ]
nTile = len(tbl)

# Points
rowContainer = [[0 for i in range(2)] for j in range(nTile)]
colContainer = [[0 for i in range(2)] for j in range(nTile)]
diagContainer = [[0,0],[0,0]]

# 0 = X, 1 = O
currTurn = 1

def setCurrTurn():
    if currTurn >= 2:
        currTurn = 1
    else:
        currTurn += 1


# doesn't keep track of last move
def check():
    for i in range(nTile):
        for j in range(nTile):
            if tbl[i][j] != currTurn: continue
            
            for a in range(-1,2):
                if i + a < 0 or i + a > len(tbl)-1: continue

                for b in range(-1,2):
                    if (b == 0 and a == 0) or j + b < 0 or j + b > len(tbl)-1: continue
                    if tbl[i + a][j + b] != currTurn: continue
                    newPos = [i + a,j + b]
                    ctr = 0
                    for k in range(nTile - 2):
                        rPos = newPos[0] + a
                        wPos = newPos[1] + b
                        if rPos < 0 or rPos > len(tbl)-1 or wPos + b < 0 or wPos > len(tbl) -1: break
                        if (tbl[rPos][wPos] != currTurn): break
                        ctr += 1
                        newPos[0] += a
                        newPos[1] += b
                    if ctr == nTile - 2:
                        return True
    return False

def checkWin():
    pass

def draw():
    print("-"*9)
    print(f"{tbl_char[tbl[0][0]]} | {tbl_char[tbl[0][1]]} | {tbl_char[tbl[0][2]]}")
    print("-"*9)
    print(f"{tbl_char[tbl[1][0]]} | {tbl_char[tbl[1][1]]} | {tbl_char[tbl[1][2]]}")
    print("-"*9)
    print(f"{tbl_char[tbl[2][0]]} | {tbl_char[tbl[2][1]]} | {tbl_char[tbl[2][2]]}")
    print("-"*9)

def printContainer():
    print(f"Row Container: {rowContainer}")
    print(f"Col Container: {colContainer}")
    print(f"Diagonal Container: {diagContainer}\n")

def ctrPoints():
    # row and col
    for i in range(nTile):
        for j in range(nTile):
            # row
            pPos = tbl[i][j]
            if pPos <= 0: continue #Bound Check
            rowContainer[i][pPos-1] += 1

            # col
            colContainer[j][pPos-1] += 1

    # diag
    slDiag = [0,0]
    baDiag = [0,nTile-1]
    for i in range(nTile):
        slDiagTbl = tbl[slDiag[0]][slDiag[1]]
        if slDiagTbl <= 0: continue
        diagContainer[0][slDiagTbl - 1] += 1
        slDiag[0] += 1
        slDiag[1] += 1

        baDiagTbl = tbl[baDiag[0]][baDiag[1]]
        if baDiagTbl <= 0: continue
        diagContainer[1][baDiagTbl - 1] += 1
        baDiag[0] += 1
        baDiag[1] -= 1
    evalWinner()

# return player code 
# 1 = 'X' 
# 2 = 'O'
def evalWinner() -> int:
    for i in range(nTile):
        for j in range(2):
            num = rowContainer[i][j]
            if rowContainer[i][j] == nTile or colContainer[i][j] == nTile: return j + 1

            if i >= 2: continue
            if diagContainer[i][j] == nTile: return j + 1

def resetContainer():
    for i in range(nTile):
        for j in range(2):
            rowContainer[i][j] = 0
            colContainer[i][j] = 0
            if i >= 2: continue
            diagContainer[i][j] = 0



def move(row: int, col: int):
    tblPos = tbl[row][col]
    if tblPos != 0: print("Move Invalid")
    tbl[row][col] = currTurn
    pass

def main():
    draw()
    ctrPoints()
    printContainer()
    print(f"Player {evalWinner()} Wins")
    resetContainer()


if __name__ == "__main__":
    main()
