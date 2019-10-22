;; original sequence 1: (1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 -76 -75 -74 -73 -72 -71 -70 -69 -68 -67 -66 -65 -64 -63 -62 -61 -56 -55 -54 -53 -60 -59 -58 -57 -27 -26 -44 -43 -42 -41 45 46 47 48 -36 -35 -25 -24 -23 -22 -21 -20 -19 -18 -17 -16 -89 -88 -87 -86 -85 -84 77 78 79 80 81 82 83 90 91 92 93 94 95 96 -105 -104 -103 -102 -101 -100 -99 -98 28 29 30 31 32 33 34 -40 -39 -38 -37 49 50 51 52 -97)
;; original sequence 2: (1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 -76 -75 -74 -73 -72 -71 -70 -69 -68 -67 -66 -65 -64 -63 -62 -61 -60 -59 -58 -57 -56 -39 -38 -37 49 50 51 52 53 40 -35 -34 -33 -32 -31 -30 -29 -28 -27 -26 -44 -43 -42 -41 45 46 47 48 -36 -25 -24 -23 -22 -21 -20 -19 -18 -17 -16 -90 -89 -88 -87 -86 -85 -84 77 78 79 80 81 82 83 91 92 93 94 95 96 -55 -54 -105 -104 -103 -102 -101 -100 -99 -98 -97)
;; simplified sequence 1: (106 -56 115 -53 114 108 -35 109 107 90 113 110 112 -40 111 -97)
;; simplified sequence 2: (106 114 -56 111 53 40 -35 -112 108 109 -90 107 113 115 110 -97)
;; common subsequences: (((1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 -76 -75 -74 -73 -72 -71 -70 -69 -68 -67 -66 -65 -64 -63 -62 -61) . 106) ((-89 -88 -87 -86 -85 -84 77 78 79 80 81 82 83) . 107) ((-27 -26 -44 -43 -42 -41 45 46 47 48 -36) . 108) ((-25 -24 -23 -22 -21 -20 -19 -18 -17 -16) . 109) ((-105 -104 -103 -102 -101 -100 -99 -98) . 110) ((-39 -38 -37 49 50 51 52) . 111) ((28 29 30 31 32 33 34) . 112) ((91 92 93 94 95 96) . 113) ((-60 -59 -58 -57) . 114) ((-55 -54) . 115))
;; #safe insertions/deletions: 0
;; sequence 1 (names): ((NORMAL SUB1) (INVERTED G56) (NORMAL SUB10) (INVERTED G53) (NORMAL SUB9) (NORMAL SUB3) (INVERTED G35) (NORMAL SUB4) (NORMAL SUB2) (NORMAL G90) (NORMAL SUB8) (NORMAL SUB5) (NORMAL SUB7) (INVERTED G40) (NORMAL SUB6) (INVERTED G97))
;; sequence 2 (names): ((NORMAL SUB1) (NORMAL SUB9) (INVERTED G56) (NORMAL SUB6) (NORMAL G53) (NORMAL G40) (INVERTED G35) (INVERTED SUB7) (NORMAL SUB3) (NORMAL SUB4) (INVERTED G90) (NORMAL SUB2) (NORMAL SUB8) (NORMAL SUB10) (NORMAL SUB5) (INVERTED G97))

(DEFINE (PROBLEM ASYNEUMA-TO-SYMPHYANDRA)
        (:DOMAIN GENOME-EDIT-DISTANCE)
        (:OBJECTS SUB10 SUB9 SUB8 SUB7 SUB6 SUB5 SUB4 SUB3 SUB2 SUB1
            G97 G90 G56 G53 G40 G35)
        (:INIT (NORMAL SUB1) (INVERTED G56) (NORMAL SUB10)
               (INVERTED G53) (NORMAL SUB9) (NORMAL SUB3)
               (INVERTED G35) (NORMAL SUB4) (NORMAL SUB2) (NORMAL G90)
               (NORMAL SUB8) (NORMAL SUB5) (NORMAL SUB7) (INVERTED G40)
               (NORMAL SUB6) (INVERTED G97) (PRESENT SUB1)
               (PRESENT G56) (PRESENT SUB10) (PRESENT G53)
               (PRESENT SUB9) (PRESENT SUB3) (PRESENT G35)
               (PRESENT SUB4) (PRESENT SUB2) (PRESENT G90)
               (PRESENT SUB8) (PRESENT SUB5) (PRESENT SUB7)
               (PRESENT G40) (PRESENT SUB6) (PRESENT G97) (CW G97 SUB1)
               (CW SUB6 G97) (CW G40 SUB6) (CW SUB7 G40) (CW SUB5 SUB7)
               (CW SUB8 SUB5) (CW G90 SUB8) (CW SUB2 G90)
               (CW SUB4 SUB2) (CW G35 SUB4) (CW SUB3 G35)
               (CW SUB9 SUB3) (CW G53 SUB9) (CW SUB10 G53)
               (CW G56 SUB10) (CW SUB1 G56) (IDLE) (= (TOTAL-COST) 0))
        (:GOAL (AND (NORMAL SUB1) (NORMAL SUB9) (INVERTED G56)
                    (NORMAL SUB6) (NORMAL G53) (NORMAL G40)
                    (INVERTED G35) (INVERTED SUB7) (NORMAL SUB3)
                    (NORMAL SUB4) (INVERTED G90) (NORMAL SUB2)
                    (NORMAL SUB8) (NORMAL SUB10) (NORMAL SUB5)
                    (INVERTED G97) (CW G97 SUB1) (CW SUB5 G97)
                    (CW SUB10 SUB5) (CW SUB8 SUB10) (CW SUB2 SUB8)
                    (CW G90 SUB2) (CW SUB4 G90) (CW SUB3 SUB4)
                    (CW SUB7 SUB3) (CW G35 SUB7) (CW G40 G35)
                    (CW G53 G40) (CW SUB6 G53) (CW G56 SUB6)
                    (CW SUB9 G56) (CW SUB1 SUB9)))
        (:METRIC MINIMIZE (TOTAL-COST)))