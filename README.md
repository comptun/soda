# soda
Soda Programming Language - A sorta Javascript inspired language

Example Program:

```javascript
import "io"
class Point {
  var x;
  var y;
  Point(x, y) {
    this.x = x;
    this.y = y;
  }
}
function main() {
  var pos = new Point(100, 200);
  con.out("X: ", pos.x, " Y: ", pos.y, "\n");
}
main();
```