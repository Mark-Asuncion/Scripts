fn main(){
    for i in 1..30 {
        let fibo = fibonacci(i);
        println!("Fibonacci of {i} is {fibo}.");
        let fibo = recur_fibonacci(i);
        println!("Recursion Fibonacci of {i} is {fibo}.\n");
    }
    let x = String::from("Hello");
    drop(x);

    println!("{}",x)
}

fn fibonacci(n: u32) -> u32{
    if n == 1 || n == 2 {
        return 1;
    }
    let mut prev = 1;
    let mut curr = 1;
    let mut i = 3;
    while i <= n {
        let tmp = curr;
        curr += prev;
        prev = tmp;
        i+=1;
    }
    curr
}

fn recur_fibonacci(n: u32) -> u32 {
    if n == 1 || n == 2 {
        return 1
    }
    recur_fibonacci(n - 1) + recur_fibonacci(n - 2)
}
