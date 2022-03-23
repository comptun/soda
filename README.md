# soda
Soda Programming Language - A sorta Javascript inspired language

Example Program:

```javascript
import "std/io"
import "std/list"

var map = new List(
    new List(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    new List(0, 1, 1, 0, 0, 0, 0, 1, 1, 0),
    new List(0, 1, 1, 0, 0, 0, 0, 1, 1, 0),
    new List(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    new List(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    new List(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    new List(0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    new List(0, 0, 1, 0, 0, 0, 0, 1, 0, 0),
    new List(0, 0, 0, 1, 1, 1, 1, 0, 0, 0),
    new List(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
);

var y = 0;
while (y < map.size()) {
    var x = 0;
    while (x < map.at(y).size()) {
        console.out(map.at(y).at(x), " ");
        x = x + 1;
    }
    console.out(endl());
    y = y + 1;
}
```