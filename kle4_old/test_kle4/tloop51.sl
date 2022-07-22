require ("slsmg");

variable p=0;
variable l=0;

slsmg_write_string ("start");

variable i;
variable j;

_for (1, 101, 1){ 
	i = ();
	_for ( 1, 800001,1) { 
		j = ();
		l = i*j+32.1 ;
		p = p+l;
	} ;
	slsmg_write_string ("loop");
}

slsmg_write_string ("end !!!!");

