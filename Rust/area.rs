#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}
fn area(n: &Rectangle) -> u32 {
    n.width * n.height
}
fn main() {
    let rec = Rectangle {
        width: 30,
        height: 30,
    };
    let res = area(&rec);
    println!("{rec:?} {res}");
    dbg!(&rec);
}
