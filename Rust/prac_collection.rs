#![allow(unused)]
use std::vec::Vec;
use std::collections::HashMap;

fn main() {
    let mut vals = vec![5,3,1,2,7,2,10,8,6];

    println!("Array: {vals:?}\nlen: {}",vals.len());
    let res = median(&vals);
    println!("Median: {res}");

    let res = mode(&vals);
    println!("Mode: {res}");
}

fn median(n: &Vec<i32>) -> i32 {
    let mut x: Vec<i32> = n.clone();
    x.sort();
    let half: usize = x.len() / 2 as usize;
    if (half) % 2 == 0 {
        return x[half];
    }
    x[half] + x[half+1] / 2 as i32
}

fn mode(n: &Vec<i32>) -> i32 {
    let mut map: HashMap<i32,i32> = HashMap::new();
    for (i,val) in n.iter().enumerate() {
        let ctr = map.entry(*val).or_insert(0);
        *ctr+=1;
    }
    let mut max = 0;
    for (_,(key,val)) in map.iter().enumerate() {
        if *val >= max { max = *val }
    }
    max
}
