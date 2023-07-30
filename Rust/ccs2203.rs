#![allow(unused)]

use std::io::Write;
pub fn clrscr() {
    println!("\x1B[2J\x1B[H");
}
pub fn cont() {
    print!("Press any key to continue...");
    std::io::stdout().flush().unwrap();
    let mut str_input = String::new();
    std::io::stdin().read_line(&mut str_input);
    std::io::stdout().flush().unwrap();
}
pub fn input<T:std::ops::Add + std::str::FromStr + std::fmt::Debug>(message: &str,n: &mut T) 
where <T as std::str::FromStr>::Err: std::fmt::Debug {
    print!("{message} ");
    std::io::stdout().flush().unwrap();

    let mut str_input = String::new();
    let err_code = match std::io::stdin().read_line(&mut str_input) {
        Ok(n) => n,
        Err(e) => {
            println!("Error {e}");
            0
        },
    };
    if err_code == 0 {
        return input(message,n);
    }
    let tokens: Vec<&str> = str_input.trim().split(|n| {
        n == ' ' || n == '\n'
    })
        .collect();
    if tokens[0] == "" {
        println!("Invalid Input, input is whitespace");
        cont();
        return input(message,n);
    }
    *n = tokens[0].parse().unwrap();
}
use std::vec::Vec;
pub mod color {
    pub const NORMAL: &str = "\x1B[0m";
    pub const BOLD: &str = "\x1B[1m";
    pub const BOLD_FORE_RED: &str = "\x1B[1;31m";
    pub const BOLD_FORE_GREEN: &str = "\x1B[1;32m";
    pub const BOLD_FORE_YELLOW: &str = "\x1B[1;33m";
    pub const FORE_WHITE_BACK_GREEN: &str = "\x1B[1;37;42m";
    pub const FORE_WHITE_BACK_RED: &str = "\x1B[1;37;41m";
    pub const FORE_CYAN_BACK_GREEN: &str = "\x1B[1;36;42m";
    pub const FORE_CYAN_BACK_RED: &str = "\x1B[1;36;41m";
    pub const FORE_RED: &str = "\x1B[0;31m";
    pub const FORE_GREEN: &str = "\x1B[0;32m"; pub const FORE_YELLOW: &str = "\x1B[0;33m";
}

pub struct Format;
impl Format {
    pub fn truncate(val: f64, length: i32) -> f64 {
        let whole_number = val.floor() as i32 as f64;
        let remainder = val - whole_number;
        const TEN: f64 = 10.0;
        let decimals = (remainder * TEN.powi(length)) as i32 as f64;
        let decimals = decimals / TEN.powi(length);
        whole_number + decimals
    }
    pub fn fround(val: f64,length: i32) -> f64 {
        const TEN: f64 = 10.0;
        let x = val * TEN.powi(length) / TEN.powi(length);
        x.ceil()
    }
}

#[derive(Debug)]
pub struct Cell {
    val: String,
    color: String,
}
impl Cell {
    pub fn nempty() -> Cell {
        Cell {
            val: String::new(),
            color: String::new(),
        }
    }
    pub fn new(val: &str,color: &str) -> Cell {
        Cell {
            val: val.to_string(),
            color: color.to_string(),
        }
    }
}
impl std::clone::Clone for Cell {
    fn clone(&self) -> Self {
        Cell {
            val: self.val.clone(),
            color: self.color.clone(),
        }
    }
}
pub struct Table {
    data: Vec<Vec<Cell>>,
    pub width: usize,
}
impl Table {
    pub fn dbg(&self) {
        dbg!(&self.data);
    }
    pub fn new(val: Vec<Cell>) -> Table {
        Table {
            data: {
                let mut x: Vec<Vec<Cell>> = Vec::new();
                x.push(val);
                x
            },
            width: 10,
        }
    }
    pub fn row_size(&self) -> usize { self.data.len() }
    pub fn col_size(&self) -> isize {
        match self.data.get(0) {
            Some(n) => n.len() as isize,
            None => -1,
        }
    }
    fn resize(&mut self) {
        let rowsize: usize = self.row_size();
        for i in &mut self.data {
            if i.len() == rowsize { continue; }
            i.resize(rowsize,Cell::nempty());
        }
    }
    pub fn push(&mut self, new_data: Vec<Vec<Cell>>) {
        let new_data_colsize: isize = match new_data.get(0) {
            Some(n) => n.len() as isize,
            None => -1,
        };
        if new_data_colsize == - 1 { return (); }
        for i in new_data { self.data.push(i); }
        self.resize();
    }
    pub fn push_col(&mut self,new_col: Vec<Cell>) {
        ()
    }
    pub fn print(&self) {
        // ┌─┬─┐
        // │ │ │
        // ├─┼─┤
        // │ │ │
        // └─┴─┘
        let border = {
            let mut res = String::new();
            for i in 0..self.width { res.push('─'); }
            res
        };
        let mut res = String::new();
        let top = {
            res.push('┌');
            for i in 0..(self.col_size()-1) as usize {
                res.push_str(border.as_str());
                res.push('┬');
            }
            res.push_str(border.as_str());
            res.push('┐');
            res
        };
        let bot = {
            let mut res = String::new();
            res.push('└');
            for i in 0..(self.col_size()-1) as usize {
                res.push_str(border.as_str());
                res.push('┴');
            }
            res.push_str(border.as_str());
            res.push('┘');
            res
        };
        let mid = {
            let mut res = String::new();
            res.push('├');
            for i in 0..(self.col_size()-1) as usize {
                res.push_str(border.as_str());
                res.push('┼');
            }
            res.push_str(border.as_str());
            res.push('┤');
            res
        };

        let create_cell = |cell: &Cell| -> String {
            let val = &cell.val;
            let color = &cell.color;
            let mut res = String::new();
            if val.len() >= self.width {
                res.push_str(&format!(" {}{}{}... ",color.as_str(),&val[..(self.width-5)],color::NORMAL));
                return res;
            }
            let leftpad = {
                let mut res = String::new();
                let size = (self.width - val.len()) / 2 as usize;
                for i in 0..size { res.push(' '); }
                res
            };
            let rightpad = {
                let mut res = String::new();
                let size = self.width - (leftpad.len() + val.len());
                for i in 0..size { res.push(' '); }
                res
            };
            res.push_str(&format!("{}{}{}{}{}",leftpad.as_str(),color.as_str(),val.as_str(),color::NORMAL,rightpad.as_str()));
            res
        };
        println!("{top}");
        let mut line = String::new();
        let mut count: usize = 1;
        for row in &self.data {
            line.push('│');
            for col in row {
                line.push_str(create_cell(&col).as_str());
                line.push('│');
            }
            let mut b: &String = &mid;
            if count >= self.row_size() as usize { b = &bot; }
            println!("{}\n{}",line,*b);
            line.clear();
            count+=1;
        }
    }
}
