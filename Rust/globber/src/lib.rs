#[allow(dead_code)]
#[derive(Debug)]
struct Globber {
    pattern: String,
}

#[allow(dead_code, unused)]
impl Globber {
    pub fn new(pattern: &str) -> Globber {
        Globber { pattern: pattern.to_string() }
    }

    pub fn search(&self, input: &str) -> String {
        let mut input_iter = input.char_indices();
        let mut pat_iter = self.pattern.char_indices();
        loop {
            // println!("{:?} {:?}", input_iter, pat_iter);
            let (_, in_ch) = input_iter.next().unwrap_or((0, '\0'));
            let (_, pat_ch) = pat_iter.next().unwrap_or((0, '\0'));
            // println!("{:?} {:?}", in_ch, pat_ch);
            if in_ch == '\0' && pat_ch == '\0' {
                return input.to_string();
            }
            else if in_ch == '\0' || pat_ch == '\0' {
                break;
            }

            // TODO handle []
            if pat_ch == '*' {
                let (_, mut till) = pat_iter.next().unwrap_or((0, '\0'));
                let mut inc_slash = false;
                if till == '*' {
                    inc_slash = true;
                    let s = pat_iter.next().unwrap_or((0, '\0'));
                    till = s.1;
                    if till == '\0' {
                        return input.to_string();
                    }
                }
                if !inc_slash && Globber::is_sep(in_ch) {
                    continue;
                }
                if !( till == in_ch ) {
                    if !self.skip_till(inc_slash, &mut input_iter, till) {
                        break;
                    }
                }
            }
            else if !(pat_ch == '?' || pat_ch == in_ch) {
                return "".to_string();
            }
            // println!("============");
        }
        "".to_string()
    }

    pub fn is_sep(c: char) -> bool {
        return c == '/' || c == '\\';
    }

    pub fn skip_till(&self,
        include_slash: bool,
        iter: &mut core::str::CharIndices<'_>,
        c: char) -> bool {
        let (_, ch) = iter.clone().next().unwrap_or((0, '\0'));
        if ch == '\0' {
            return false;
        }
        if !include_slash && Globber::is_sep(ch) {
            return true;
        }
        iter.next();
        if ch == c {
            return true;
        }
        return self.skip_till(include_slash, iter, c);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    fn get_list() -> Vec<String> {
        return vec!["file.txt".to_string(),
            "document.txt".to_string(),
            "image.png".to_string(),
            "file1.txt".to_string(),
            "fileA.txt".to_string(),
            "file12.txt".to_string(),
            "folder/File.java".to_string(),
            "folder/subfolder/AnotherFile.java".to_string(),
            "prefix1.txt".to_string(),
            "prefix5.png".to_string(),
            "prefixA.doc".to_string()
        ]
    }

    #[test]
    fn glob_1() {
        let list = get_list();
        let globber = Globber::new("*.txt");
        let mut res: Vec<String> = vec![];
        for m_str in &list {
            let s = globber.search(m_str);
            if !s.is_empty() {
                res.push(s);
            }
        }

        let ans = [0,1,3,4,5,8];
        let mut ans_res: Vec<&str> = vec![];
        for i in ans {
            ans_res.push(list.get(i as usize).unwrap());
        }
        assert_eq!(&ans_res, &res);
    }
    #[test]
    fn glob_2() {
        let list = get_list();
        let globber = Globber::new("file?.txt");
        let mut res: Vec<String> = vec![];
        for m_str in &list {
            let s = globber.search(m_str);
            if !s.is_empty() {
                res.push(s);
            }
        }

        let ans = [3,4];
        let mut ans_res: Vec<&str> = vec![];
        for i in ans {
            ans_res.push(list.get(i as usize).unwrap());
        }
        assert_eq!(&ans_res, &res);
    }

    #[test]
    fn glob_3() {
        let list = get_list();
        let globber = Globber::new("?older*/*.java");
        let mut res: Vec<String> = vec![];
        for m_str in &list {
            let s = globber.search(m_str);
            if !s.is_empty() {
                res.push(s);
            }
        }

        let ans = [6];
        let mut ans_res: Vec<&str> = vec![];
        for i in ans {
            ans_res.push(list.get(i as usize).unwrap());
        }
        assert_eq!(&ans_res, &res);
    }

    #[test]
    fn glob_4() {
        let list = get_list();
        let globber = Globber::new("?older**.ja**");
        let mut res: Vec<String> = vec![];
        for m_str in &list {
            let s = globber.search(m_str);
            if !s.is_empty() {
                res.push(s);
            }
        }

        let ans = [6,7];
        let mut ans_res: Vec<&str> = vec![];
        for i in ans {
            ans_res.push(list.get(i as usize).unwrap());
        }
        assert_eq!(&ans_res, &res);
    }

    #[test]
    fn glob_5() {
        let list = get_list();
        let globber = Globber::new("i?age*e");
        let mut res: Vec<String> = vec![];
        for m_str in &list {
            let s = globber.search(m_str);
            if !s.is_empty() {
                res.push(s);
            }
        }
        dbg!(&res);
        assert!(res.is_empty());
    }
}
