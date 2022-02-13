# Protocols

All numerical data should converted to a UTF-8 encoded string.

Each Value should be terminated with a '_' character

## Footers
All requests end with ')'


## Connection

### Header: 
CONN_

### Definitions
ID - Machine ID (Integer)

### Example:
CONN(ID843128610)

Connection from machine 843128610

## Connection Response

### Header:
ACCP(

### Definitions
PN - Player Number (Integer)

### Example
ACCP(PN1)

Accepted connection. You are player 1.

## Sending

### Header: 
INPT(

<br>

ID - Machine ID (Integer)


### Definitions:

L - LEFT

R - RIGHT

J - JUMP

S - SHOOT

E - READY

All are boolean values.


### Example

INPT(ID843128610_L0_R1_J0_S1_E1_)

User input from machine 843128610
Shows that the user isn't pressing: Left or Jump but is pressing Right and Shoot. And that the user is ready.

## Recieving

There can be any number of actors.

### Header:
DATA-

### Block Types
A - Actor

### Definitions
PX - X position (float)

PY - Y position (float)

SX - X speed (float)

SY - Y speed (float)

<br>

( - Actor Start

) - Actor End

AT - Actor Type

AN - Actor Number

AX - Actor State

### Actor Types
0 - Player

1 - Bullet

Actor Number corresponds to player numbers

### Actor States

0 - Alive

1 - Dead

### Example

DATA(A(AT0_AN1_AX0_PX100_PY100_SX1_SY0)A(AT1_AN0_AX1_PX50_PY10_SX0_SY10))

| Actor     | X   | Y   | Other  |
|--------------|-----|-----|--------|
| Actor Type   |     |     | Player |
| Actor Number |     |     | 1 |
| Actor State |     |     | Alive |
| Position     | 100 | 100 |        |
| Speed        | 1   | 0   |        |

| Actor     | X  | Y  | Other  |
|--------------|----|----|--------|
| Actor Type   |    |    | Bullet |
| Actor Number |    |    | 0      |
| Actor State |     |     | Dead |
| Position     | 50 | 10 |        |
| Speed        | 0  | 10 |        |