(defrule r1 "night time low water" 
(time is night) => (set_result low))

(defrule r2 "" 
(weather is hot) (day is off) (or (time is day) (time is evening)) (veg is vegeance) => (set_result higher))

(defrule r3 "" 
(weather is hot) (day is off) (or (time is day) (time is evening)) (veg is novegeance) => (set_result high))

(defrule r4 "" 
(weather is cold) (day is off) (or (time is day) (time is evening)) => (set_result high))

(defrule r5 "" 
(weather is hot) (day is off) (time is morning) => (set_result high))

(defrule r6 "" 
(weather is cold) (day is off) (time is morning) => (set_result medium))

(defrule r7 "" 
(weather is hot) (day is on) (time is morning) => (set_result high))

(defrule r8 "" 
(weather is cold) (day is on) (time is morning) => (set_result medium))

(defrule r9 "" 
(weather is hot) (day is on) (time is day) (veg is vegeance) => (set_result medium))

(defrule r10 "" 
(weather is hot) (day is on) (time is day) (veg is novegeance) => (set_result low))

(defrule r11 "" 
(weather is cold) (day is on) (time is day)=> (set_result low))

(defrule r12 "" 
(day is on) (time is evening)=> (set_result high))

(defrule r13 "" 
(weather is cold) (day is on) (time is evening)=> (set_result medium))