# Protocols

## Sending

### Header: 
INPUT-

### FOOTER
-END

### Definitions:
L - LEFT

R - RIGHT

J - JUMP

S - SHOOT

E - READY

All are boolean values.

### Example

INPUT-L0R1J0S1E1-END

Shows that the user isn't pressing: Left or Jump but is pressing Right and Shoot. And that the user is ready.

## Recieving

There can be any number of actors.

### Header:
DATA-

### Footer:
-END

### Definitions
PX - X position

PY - Y position

SX - X speed

SY - Y speed

<br>

AS - Actor Start

AE - Actor End

AT - Actor Type

AN - Actor Number

### Actor Types
0 - Player

1 - Bullet

Actor Number corresponds to player numbers

### Example

DATA-ASAT0AN1PX100PY100SX1SY0AEASAT1AN0PX50PY10SX0SY10AE-END

|              | X   | Y   | Other  |
|--------------|-----|-----|--------|
| Actor Type   |     |     | Player |
| Actor Number |     |     | 1 |
| Position     | 100 | 100 |        |
| Speed        | 1   | 0   |        |

|              | X  | Y  | Other  |
|--------------|----|----|--------|
| Actor Type   |    |    | Bullet |
| Actor Number |    |    | 0      |
| Position     | 50 | 10 |        |
| Speed        | 0  | 10 |        |