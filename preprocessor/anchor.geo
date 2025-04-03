w  = 1;
h  = 1;
r  = 4*w;
R = 6*w;
lc = 0.1;


Point(1) = { 0, 0, 0, lc};
Point(2) = {-r, 0, 0, lc};
Point(3) = { r, 0, 0, lc};

Point(4) = { -R, 0, 0, lc};
Point(5) = { R, 0, 0, lc};

Point(6) = {w, 0,  0, lc};
Point(7) = {-w, 0, 0, lc};

Point(8) = {w, -R/2,  0, lc};
Point(9) = {-w, -R/2, 0, lc};

Point(10) = {-w, r/2, 0, lc};
Point(11) = { w, r/2, 0, lc};

Point(12) = {-r/2, r/2, 0, lc};
Point(13) = { r/2, r/2, 0, lc};
Point(14) = { r/2, R/2, 0, lc};
Point(15) = {-r/2, R/2, 0, lc};
Point(16) = { w, R/2, 0, lc};
Point(17) = {-w, R/2, 0, lc};

Point(18) = {-w, r, 0, lc};
Point(19) = { w, r, 0, lc};

Point(20) = { 0, R-w/2, 0, lc};
Point(21) = { 0, Hypot(r/2-w/2, w) + R-w/2, 0, lc };

Point(22) = { w-R, w, 0, lc};
Point(23) = { R-w, w, 0, lc};

Point(24) = { -R-w, 0, 0, lc};
Point(25) = { -r+w, 0, 0, lc};

Point(26) = { r-w, 0, 0, lc};
Point(27) = { R+w, 0, 0, lc};

Point(28) = { 0, -R, 0, lc};

Point(29) = { 0, 5 , 0, lc };
Point(30) = { 0.5, 5.5 , 0, lc };
Point(31) = { 0, 6 , 0, lc };
Point(32) = { -0.5 , 5.5 , 0, lc };

Circle(1) = {2, 7, 9};
Circle(2) = {8, 6, 3};

Circle(3) = {5, 1, 28};
Circle(17) = {28, 1, 4};

Line(4) = {9, 10};
Line(5) = {11, 8};

Line(6) = {10, 12};
Line(7) = {12, 15};
Line(8) = {15, 17};

Line(9) = {16, 14};
Line(10) = {14, 13};
Line(11) = {13, 11};

Line(12) = {17, 18};
Line(13) = {19, 16};

Circle(14) = {18, 20, 21};
Circle(15) = {21, 20, 19};

Line(16) = {4, 24};
Line(18) = {24, 22};
Line(19) = {22, 25};
Line(20) = {25, 2};

Line(21) = {3, 26};
Line(22) = {26, 23};
Line(23) = {23, 27};
Line(24) = {27, 5};

Circle(25) = {29, 20, 30};
Circle(26) = {30, 20, 31};
Circle(27) = {31, 20, 32};
Circle(28) = {32, 20, 29};

Curve Loop(1) = {1, 4, 6, 7, 8, 12, 14, 15, 13, 9, 10, 11, 5, 2, 21, 22, 23, 24, 3, 17, 16, 18, 19, 20};
Curve Loop(2) = {25, 26, 27, 28};
Plane Surface(1) = {1, 2};