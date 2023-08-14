use std::vec::Vec;

fn main() {
    let arr = vec![1,3,4,7,11,16,20,23,25,30];
    let find = 23;
    let ans = binary_search(&arr,find);
    println!("Index of 23 is {ans}");
}

fn binary_search(arr: &Vec<i32>,n: i32) -> i32 {
    let mut start: i32 = 0;
    let mut end: i32 = arr.len() as i32;
    while start <= end {
        let half = ((start + end) / 2) as usize;
        let curr = *( arr.get(half).unwrap() );
        if curr == n {
            return half as i32;
        }
        else if curr < n {
            start = (half + 1) as i32;
        }
        else {
            end = (half - 1) as i32;
        }
    }
    -1
}
