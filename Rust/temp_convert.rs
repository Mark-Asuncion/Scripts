fn main() {
    let mut i = 1.0;
    while i <= 50.0 {
        let x = convert_to_celsius(i);
        println!("{i} fahrenheit is {x} celsius");
        let x = convert_to_fahrenheit(i);
        println!("{i} celsius is {x} fahrenheit\n");
        i+=0.3;
    }
}

fn convert_to_celsius(n: f64) -> f64 {
    (n - 32.0) * 5.0/9.0
}

fn convert_to_fahrenheit(n: f64) -> f64 {
    (n * (9.0/5.0)) + 32.0
}
