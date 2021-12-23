classDiagram
class Base{
<<interface>>
}
class Shape{
    <<interface>>
    noOfVertices
    draw()
}
class Color{
    <<enumeration>>
    RED
    BLUE
    GREEN
    WHITE
    BLACK
}
Base <|.. Shape
Shape o--Color

class Rect{
draw()
double width
double length
}
Shape <|-- Rect

class Circle{
draw()
r
}
Shape <|-- Circle
