module pill(_height, _radius){
union() {
translate([0,0,0]) sphere(r=_radius, $fn=32);
translate([0,0,0]) cylinder(r=_radius,h=_height, $fn=32);
translate([0,0,_height]) sphere(r=_radius, $fn=32);
}
}


module base1(_base_height,_base_radius,_knob_radius) {
union() {
cylinder(r=_base_radius, h=_base_height, $fn=64);
translate([0,0,_base_height]) cylinder(r1=_base_radius, r2=_knob_radius, h=5, $fn=64);
}
}
module base2(_base_height,_base_radius,_ring_radius) {
union() {
cylinder(r1=_base_radius+1, r2=_base_radius, h=_base_height, $fn=64);
translate([0,0,_base_height]) rotate_extrude(convexity = 10, $fn=64)
translate([_base_radius-_ring_radius, 0, 0])
circle(r = _ring_radius, $fn=32);
translate([0,0,_base_height]) cylinder(r=_base_radius-_ring_radius, h=_ring_radius, $fn=64);
}
}

base_height=2.5;
base_radius=8;

knob_height=14;//16
knob_radius=5.5;

shaft_radius=3.22;//3.18;//3.1
nut_radius=7.2;
nut_height=2.7;
shaft_height=8.8;//16
shaft_height_total=13;//18


rotate([0,180,0]) {
    difference(){
union(){
base1(base_height, base_radius, 1);

translate([0,0,base_height]) {
difference() {
base2(knob_height, knob_radius, 1.5);
for (i=[0:30:360]) {
   rotate([0,0,i]) translate([knob_radius+1,0,3]) rotate([0,-3.5,0]) pill(knob_height+1, 1);
}
}
}
}
union() {
cylinder(r=nut_radius, h=nut_height,$fn=32);
difference() {
    union() {
cylinder(r=shaft_radius, h=shaft_height_total,$fn=32);

cylinder(r=shaft_radius+0.35, h=shaft_height_total-shaft_height,$fn=32);
    }
//#translate([1.9,-shaft_radius,shaft_height_total-shaft_height]) //cube([2*shaft_radius,2*shaft_radius,shaft_height]);
}
}
}

}