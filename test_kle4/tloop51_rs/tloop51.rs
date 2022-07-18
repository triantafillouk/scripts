

//use std::io::stderr;

use std::io::Write;


macro_rules! eprintln(
    ($($arg:tt)*) => { {
        let r = writeln!(&mut ::std::io::stderr(), $($arg)*);
        r.expect("failed printing to stderr");
    } }
);

fn main() {
	println!("Start of test loop 51");
	let mut p: f64 =0.0;
//	let mut l: f64 =0.0;
	for i in 1..101 {
		for j in 1..800001 {
			let s=(i*j) as f64;
//			l = s +32.1;
			p = p + s + 32.1;
		};
		println!("loop: {} p={}",i,p)
	};
	eprintln!("End test p={}",p);
}