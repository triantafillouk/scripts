<?
    // version 2.7 (5/10/2007)
    // version 2.6 (28/9/2007) show both deltas if actual ots exists
    // version 2.5 (20/9/2007) take ots from actual or predicted desmhe!!!
    // version 2.4 (3/8/2007) show mean for efficiency!!
    // version 2.3 (17/7/2007) changes in table showings
    // version 2.2 (12/7/2007) added support for off hours, css in table
    // version 2.1
    // added support for type of day and hour range
    // version 2.2 (15/10/2008) added a new column with co2
    
    require 'db_utils.php';
    require 'page_headers.php';
    require 'date_utils.php';
	  require_once("show_template.php");
$prev_page="input41.php";
$ref=getenv('HTTP_REFERER');
if(basename($ref) != $prev_page)
{
  header("Location: ".$prev_page);
  exit;
} 

/*
 Επιλογές:
 //GR peranei 1 string kathe date pia
 'day1' (1-24) from selection
 'month1' (ΙΑΝ .. ΔΕΚ) from selection
 'year1' (2005 - 2009) from selection

 'day2' (1-24) from selection
 'month2' (ΙΑΝ .. ΔΕΚ) from selection
 'year2' (2005 - 2009) from selection

 'w_ews' ώρες έως (0-23) from selection
 'w_apo' ώρες από (0-23) from selection
 
*/    

function disp_array($htitles,$vtitles,$vals,$htotals=false,$vtotals=false) {
    $hsize = sizeof($htitles);
    $vsize = sizeof($vtitles);
    // print "hsize=$hsize, vsize=$vsize<br>";
    print "<table border=1 cellpadding=3 cellspacing=0 class=text>";
    print "<tr><td>&nbsp </td>";
    for($h=0;$h<$hsize;$h++){ 
        $ht=$htitles[$h];
        print "<td class=hheader>$ht</td>";
    };
    if($vtotals) {
        $t=$vtotals[0];
        print "<td align='center'>$t</td>";
    }
    print "</tr>";
    for($v=0;$v<$vsize;$v++) {
        $vt=$vtitles[$v];
        print "<tr><td class='vheader' align='center'>$vt</td>";
        for($h=0;$h<$hsize;$h++){
             $val=$vals[$v][$h];
            print "<td align='center'>" . number_format($val,2,',','.') . "</td>";  
        };
        if($vtotals) {
            $t = $vtotals[$v+1];
            print "<td class='vsum' align='center'>". number_format($t,2,',','.') . "</td>";
        }
        print "</tr>";
    };
    if($htotals) {
        print "<tr><td>&nbsp;</td>";
        for($h=1;$h<=$hsize;$h++){
            $t=$htotals[$h];
            print "<td class='hsum' align='center'>". number_format($t,2,',','.') . "</td>";
        }
        print "</tr>";
    }
    print "</table><br>";
}


 
 if(1){
 $year1=substr($complete_date1,6,4);
 $month1=substr($complete_date1,3,2);
 $day1=substr($complete_date1,0,2);

 $year2=substr($complete_date2,6,4);
 $month2=substr($complete_date2,3,2);
 $day2=substr($complete_date2,0,2);

 } else {
 $year1=substr($complete_date1,0,4)+0;
 $month1=substr($complete_date1,4,2)+0;
 $day1=substr($complete_date1,6,2)+0;

 $year2=substr($complete_date2,0,4)+0;
 $month2=substr($complete_date2,4,2)+0;
 $day2=substr($complete_date2,6,2)+0;
 };

 $_SESSION['fulldate1']=$day1."/".$month1."/".$year1;
 $_SESSION['fulldate2']=$day2."/".$month2."/".$year2;

 $report_name="salonica_hourly_".$day1.$month1.$year1."-".$day2.$month2.$year2;
 show_header("Κίνηση μονάδας Θεσσαλονίκης",3);
 print "<div align='left'>";
 echo "<p class=section>Κίνηση μονάδας Θεσσαλονίκης - Ωριαία αθροίσματα</p>";

 $check_normal=isset($normaldays);
 $check_sabato=isset($sabato);
 $check_kyriaki=isset($kyriaki);
 
//  show input values
if(false){
 echo 'day1=',$day1,' ';
 echo 'month1 = ',$month1,' ';
 print "year1=$year1<br>";
 echo 'day2=',$day2,' ';
 echo 'month2 = ',$month2,' ';
 print "year2=$year2<br>";
 
 print "Normal days=$check_normal Σάββατο=$check_sabato Κυριακή=$check_kyriaki<br>";
 print "w_apo = $w_apo ";
 print "w_ews = $w_ews<br>"; 
  
};

 //$month1 = month_from_greek($month1);
 //$month2 = month_from_greek($month2);

    // find the real number of days between dates
    $number_of_days=0;
    $current_day=$day1;     // this is first day to count
    $current_month=$month1;
    $current_year=$year1;
    while(1){
       // print "check $current_day / $current_month / $current_year : $number_of_days<br>";
        if(cal_days_in_month(CAL_GREGORIAN, $current_month, $current_year)<$current_day){
            $current_day=1;
            $current_month++;
            if($current_month>12){
                $current_month=1;
                $current_year++;
            }
        };
        if($current_year>$year2) break;
        if($current_year==$year2) {
            if($current_month>$month2) break;
            if($current_month==$month2) {
                if($current_day>$day2) break;
                if($current_day==$day2) {$number_of_days++;break;};
            }
        }
        $number_of_days++;
        $current_day ++;
        if($number_of_days>1000) break;    
    }
    $real_days = $number_of_days;
    // print "real_days = $real_days<br>";
    // ------------------------------------------
 
 $dat1 = "$year1/$month1/$day1";
 $dw1 = day_of_week($dat1);
 $gdw1 = day_name($dw1);
 $dat2 = "$year2/$month2/$day2";
 $dw2 = day_of_week($dat2);
 $gdw2 = day_name($dw2);
 
// print "dat1=$dat1 = $dw1 = $gdw1, dat2=$dat2 = $dw2 = $gdw2 <br>";
// print "number of real days is $number_of_days<br>";
// exit;
  
 print "<span class=section2>Περίοδος από $day1/$month1/$year1 έως $day2/$month2/$year2 από ώρα $w_apo έως $w_ews </span><br>
";

 if($check_normal==1) print "<span class=section2>Περιλαμβάνονται καθημερινές</span><br>";
 if($check_sabato==1) print "<span class=section2>Περιλαμβάνεται το Σάββατο</span><br>";
 if($check_kyriaki==1) print "<span class=section2>Περιλαμβάνεται η Κυριακή</span><br>";
 print "<br>";
 
 $total_hours = $w_ews-$w_apo+1;

 $conn = dsn_connect();

 $phours = array();

 if($conn) {
    // create the hour list 
    $hour_list = "";
    $hindex = array();
    $harray = array();
    for($i=0;$i<=24;$i++) $hindex[$i]=0;
    if($hour_selection != 'all') { // get peak hours table, find $total_hours
    $sql = "select peak from mhours";
        $phours = get_array($conn,$sql);
        $total_hours=0;
        if($hour_selection == 'peak')
            for($i=0,$ind=0;$i<24;$i++) {
                if($phours[$i]) {
                    if($total_hours>0) $hour_list .= ",";
                    $h = $i+1;
                    $hour_list .= "$h";
                    $total_hours++;
                    $hindex[$h] = $ind;
                    $harray[$ind++] = $h;
                };
            };
        if($hour_selection == 'off')
            for($i=0,$ind=0;$i<24;$i++) {
                if($phours[$i]==0) {
                    if($total_hours>0) $hour_list .= ",";
                    $h = $i+1;
                    $hour_list .= "$h";
                    $total_hours++;
                    $hindex[$h] = $ind;
                    $harray[$ind++] = $h;
                  };
            }
    } else  {
        $total_hours=0;
        for($i=$w_apo,$ind=0;$i<=$w_ews;$i++) {
                    if($total_hours>0) $hour_list .= ",";
                    $hour_list .= "$i";
                    $total_hours++;
                    $hindex[$i] = $ind;
                    $harray[$ind++] = $i;
        }
    };
    // end of creation of hour list!

    // find the real number of days
    
    // get ots actual
    $ftype = "actual";
    $sql = "select dat,hour ";
    $sql = $sql . ",ots_$ftype ";
    $sql = $sql . " from ftable1_$ftype "; 
    $sql .= " where dat >= #$dat1# and dat <= #$dat2#";
    $sql .= " and hour in ( $hour_list ) ";
	$sql = $sql . " order by dat,hour";
    $actual_var = get_array($conn,$sql);
    $rows = sizeof($actual_var);
    $a_days = $rows / $total_hours;
    if($a_days < $real_days) {
        // print "a_days = $a_days < selected days = $real_days<br>";
        $a_days=0;
    } else {
    // create the array of prices ($real_days x $total_hours)
    $actual_ots = array();
     for($d=0,$r=0;$d<$real_days;$d++) for($h=0;$h<$total_hours;$h++,$r++) { 
        $actual_ots[$d][$h]=$actual_var[$r][2];
     };
    };

    { // if there is no actual prices check for predicted
    // get ots predicted
    $ftype = "pred";
    $sql = "select dat,hour ";
    $sql = $sql . ",ots_$ftype ";
    $sql = $sql . " from ftable1_$ftype "; 
    $sql .= " where dat >= #$dat1# and dat <= #$dat2#";
    $sql .= " and hour in ( $hour_list ) ";
	$sql = $sql . " order by dat,hour";
    $pred_var = get_array($conn,$sql);
    $rows = sizeof($pred_var);
    $p_days = $rows / $total_hours;
    if($p_days < $real_days) {
       // print "p_days = $p_days < selected days = $real_days<br>";
        $p_days=0;
    } else {
    // create the array of prices ($real_days x $total_hours)
    $pred_ots = array();
     for($d=0,$r=0;$d<$real_days;$d++) for($h=0;$h<$total_hours;$h++,$r++) { 
        $pred_ots[$d][$h]=$pred_var[$r][2];
     };
    };
    }
    $p_days=0;
    
    //$sql = "select dat,hour, net_prod_actual,gas_cons_actual,gas_cons_lvh,efficiency_lvh,power_sales_actual,power_sales_expo_actual,gas_cost_actual,delta_sales_gas_cost_actual ";
    $sql = "select dat,hour, net_prod_actual,gas_cons_actual,gas_cons_lvh,efficiency_lvh,power_sales_actual,power_sales_expo_actual,gas_cost_actual,co2,delta_sales_gas_cost_actual ";
    $sql .= " from ftable4";
    $sql .= " where dat >= #$dat1# and dat <= #$dat2#";
    $sql .= " and hour in ( $hour_list ) ";
	$sql = $sql . " order by dat,hour";
     
//    if($w_apo!=1 || $w_ews!=24) {
//        if($w_ews < $w_apo) $w_ews=24;
//        $sql = $sql . " and hour >= $w_apo and hour <= $w_ews";
//    };
    // print "sql = [$sql]<br>";
    
    $vmvar = get_array($conn,$sql);
	$rows = sizeof($vmvar);
	$total_days = $rows / $total_hours;
	
    if($total_days==0) {
        print "Δέν υπάρχουν στοιχεία!<br>";
        print "</div>";
        show_footer();
        exit;
    }
    // first column is day,second column is hour
	$net_cid=2;
	$gas_cid=3;
	$gas_lvh_cid=4;
	$efficiency_cid=5;
	$ps_cid=6; // power_sales_predicted
	$pse_cid=7; // power_sales_expo_actual
	$gc_cid=8;  // gas_cost_actual
	$co2_cid=9; // co2 surplus
    $dsgc_cid=10; // delta_sales_gas_cost_actual
	
	// create table 
    // print "rows=$rows, total days found=$total_days, total_hours=$total_hours <br>";
    
    if($total_days< $real_days) print "Προσοχή! έχουν επιλεγεί $real_days ημέρες αλλά υπάρχουν στοιχεία μόνον για $total_days!<br><br>";
    // initialize table
    for($h=0;$h<$total_hours;$h++) {
        $vtitles[$h]=$h+1;
        for($i=0;$i<9;$i++) {
            $par1[$h][$i]=0.0;
        }
    };


    $row=0;    
    $actual_days=0; // in case not all days are summed
    $current_day=$day1;     // this is first day to count
    $current_month=$month1;
    $current_year=$year1;
    
    for($d=0;$d<$total_days;$d++,$current_day++){
        // check for current day validity
        if(cal_days_in_month(CAL_GREGORIAN, $current_month, $current_year)<$current_day){
            $current_day=1;
            $current_month++;
            if($current_month>12){
                $current_month=1;
                $current_year++;
            }
        }
        // create a string with current date
        
        $current_date = "$current_year/$current_month/$current_day";
        $cwd = day_of_week($current_date);
        $name_cwd = day_name($cwd);
//        print "compute day $current_date, cwd=$cwd = $name_cwd, <br>";
//        if($check_normal) print "check_normal<br>" ;
//        else print "DO NOT !!!! check_normal<br>" ;
        if(!$check_normal && $cwd>0 && $cwd<7 ) continue;
        if(!$check_sabato && $cwd==7 ) continue;
        if(!$check_kyriaki && $cwd==0) continue;
    
        $day = $vmvar[$row][0]; //GR eixe row anti gia $row
  //      print "day $d : $day <br>";
        for($h=0;$h<$total_hours;$h++) {
            $par1[$h][0] += $vmvar[$row][$net_cid];             // production net
            if($vmvar[$row][$gas_cid]>0)
                $par1[$h][1] += $vmvar[$row][$gas_cid];             // gas consuption Nm3
            if($vmvar[$row][$gas_lvh_cid]>0)
                $par1[$h][2] += $vmvar[$row][$gas_lvh_cid];          // gas consuption MWh
//            if($vmvar[$row][$efficiency_cid]>0)
//                $par1[$h][3] += $vmvar[$row][$efficiency_cid];      // efficiency
            if($p_days>0) {
                $par1[$h][4] += $pred_ots[$d][$h]*0.975* $vmvar[$row][$net_cid];
             } else {
               $par1[$h][4] += $vmvar[$row][$ps_cid];              // power sales DAS
            };
            if($a_days>0) {
                $par1[$h][5] += $actual_ots[$d][$h]*0.975* $vmvar[$row][$net_cid];
            } else {
                $par1[$h][5] += $vmvar[$row][$pse_cid];             // power sales expost
            };

            
            $par1[$h][6] += $vmvar[$row][$gc_cid];              // gas cost
            $par1[$h][7] += $vmvar[$row][$co2_cid];             // co2 cost

//            if($a_days>0) {
//                $par1[$h][8] = $par1[$h][5] - $par1[$h][6];
//            };
//            if ($p_days>0)
//            {
//                $par1[$h][7] = $par1[$h][4] - $par1[$h][6];
//            } else {
////               $par1[$h][7] += $vmvar[$row][$dsgc_cid];            // delta sales gas cost
//               $par1[$h][7] = $vmvar[$row][$ps_cid] - $vmvar[$row][$gc_cid];            // delta sales gas cost
//            };
           
            $row++; 
        }
    };

    $actual_cost=0;
    for($h=0;$h<$total_hours;$h++) {
        $actual_cost += $par1[$h][5];
    }
    
    // for the efficiency we should show the mean value!!
    $production_hours=0;
    for($h=0;$h<$total_hours;$h++) {
        // $par1[$h][3] /= $total_days;
        // for the efficiency we should show the mean value!!
        if($par1[$h][2]!=0) $par1[$h][3] = 100.0 * $par1[$h][0] / $par1[$h][2];
        else $par1[$h][3] = 0;
        if($par1[$h][3]!=0) $production_hours++;
        
        $par1[$h][8] = $par1[$h][4] - $par1[$h][6] - $par1[$h][7];
        if($actual_cost>0) $par1[$h][9] = $par1[$h][5] - $par1[$h][6];
        else $par1[$h][9]=0;
        
    }
     
    // create totals
    $totals[0]="Totals";
    for($v=1;$v<=10;$v++) {
            $totals[$v] = 0;
    }
    for($v=1;$v<=10;$v++) {
        for($h=0;$h<$total_hours;$h++){
            if($par1[$h][$v-1]>0 || $v> 7)  
                $totals[$v] += $par1[$h][$v-1];
        }
    };
    // for the efficiency we should show the mean
    // $totals[4] /= $production_hours;
    if($totals[3]!=0) $totals[4] = 100*$totals[1] / $totals[3];
    else $totals[4]=0;
    $htitles = array("Net production (MWh)", "Gas Consumption Nm3", "Gas Consumption MWh LHV",
        "Efficiency  LHV(%)","Power Sales DAS (&euro;)","Power Sales EXPOST (&euro;)",
        "Gas Cost (&euro;)","CO2 deficit / surplus","Delta Sales-Gas Cost DAS (&euro;)"
        , "Delta Sales-Gas Cost EXPOST (&euro;)"
    );

    disp_array($htitles, $vtitles, $par1,$totals);
    odbc_close($conn);

    
}else {
    print "could not connect to database!";
 };
 
 print "</div>";
 show_footer();
?>
