#![allow(unused)]
use std::vec::Vec;
use std::collections::HashMap;
fn main() {
    let mut s = String::from("Hello");
    let y = &mut s;
    y.push('s');
    dbg!(y);
    let mut x = vec![1,2,3];
    for i in &mut x {
        println!("{i}");
        *i = 5;
    }
    dbg!(x);
    let mut x = 1;
    test(&mut x);
    dbg!(x);

    let mut x: Vec<Number> = Vec::new();
    x.push(Number::Float(6.4));
    x.push(Number::Int(5));
    dbg!(&x);
    for i in &x {
        match i {
            Number::Int(x) => { println!("{x}"); },
            Number::Float(x) => { println!("{x}"); },
        };
    }

    let x = Number::Int(32);
    match x {
        Number::Int(x) => { println!("{x}"); },
        Number::Float(x) => { println!("{x}"); },
    }
    let x = "literal string";
    let x = x.to_string();
    let slice: &str = &x[2..3];

    let x = String::new();
    dbg!(x);

    let hello = "Здравствуйте";
    dbg!(&hello.chars());
    // panics
    // let answer = &hello[0..1];
    // dbg!(answer);
    //
    let mut x: HashMap<&str, i32> = HashMap::new();
    x.insert("1", 35);
    x.insert("2", 78);
    dbg!(&x);
    x.insert("1", 78);
    dbg!(&x);

    let x = last_char_of_first_line("asd").unwrap();
    dbg!(x);

    let mut str_input = String::new();
    match std::io::stdin().read_line(&mut str_input) {
        Ok(n) => n,
        Err(e) => {
            println!("Error {e}");
            0
        },
    };
}
#[derive(Debug)]
enum Number {
    Int(i32),
    Float(f32),
}
fn last_char_of_first_line(text: &str) -> Option<char> {
    text.lines().next()?.chars().last()
}

fn test(x: &i32) {
    println!("{x}");
}
