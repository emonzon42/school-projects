#lang scheme
#|
Eli Monzon
3.29.20
ICSI 311
BTree
|#

#|entry to program|#
(define (main) 
  (search tree 1)
  (search tree 3)
  (search tree 140)
  (search tree 6)
  (search tree 1001)
  (search tree 2)
  (search tree 143)
  (search tree 260)
  (search tree 7)
  (search tree 10)
  (search tree 290)
  (search tree 15)
  (search tree 300)
  (search tree 9)
  (search tree 253)
  (search tree 200)
  (search tree 0)
  (search tree -6)
  (search tree 900)
  (search tree 692)
  (search tree 500)
  (search tree 5)
  (search tree 647)
)

#|searches if a key can exist in the tree|#
(define (search tree key) 
 (display key)
  (let ((max (car (cdr (cdr tree)))))
    (cond
      ((or (> key max) (< key (car(cdr tree)))) ;key is not within range of tree
        (display " is not in tree :[ (out of range)")
        (newline))
      (else ;within range so calls deepsearch to do searches node by node
       (deepSearch (car (cdr (cdr (cdr tree)))) key (car (cdr (cdr (cdr tree)))))
      )
    )
    )
)

#|searches node by node in tree until match is found/not found|#
(define (deepSearch innertree key node) 
  (cond
    
    ((null? innertree); reached end of tree
     (display " is not in tree :[ (no rootnodes w range for key)")
          (      n                     ewline)
     )
    
    ((list? (car node)); first item is a node
     (deepSearch innertree key (car node))
    )
    
    ((equal? (car node) "R"); node is a rootnode
     
     (cond
       ((> key (car (cdr (cdr node)))); key is > than max for node so check next node

        (deepSearch (cdr innertree) key (cdr innertree))); skips to next rootnode
       (else ; check through the leafnodes for match
        (cond
          ((null? (cdr (cdr (cdr node))))
           (display " is not in tree :[ (rootnode got no leaves)")
           (newline))
          ((> (length (car (cdr (cdr (cdr node))))) 1); more than one leaf, combines them into one (for easier checking)
           (let ((allLeaves (apply apply append (cdr (cdr (cdr node))))))
        (deepSearch innertree key allLeaves));allLeaves is every leafnode (within root) combined
           )
          (else; only 1 leaf node so no combination neccesary
           (deepSearch innertree key (car (cdr (cdr (cdr node))))))
          ))
       )
     )
        
    ((equal? (car node) "L"); node is a leafnode
     (cond
       ((and key (member key node))
        (display  " is in tree ! :] ")
        (newline))
       (else
        (display " is not in tree :[ ")
        (newline))
       )
     )
    )
)

#|test tree|#
(define tree ' ( "R" 0 999 
    (
        ("R" 0 99    
            (   
                ("L" 0 1 2 4 5 6 8 10)
                ("L" 14 15 19 43 93)
            )
        )    
        ("R" 100 199 
            (
                ("L" 120 140 160 180)
            )
        )       
        ("R" 200 299 
            (
                ("L" 220 240 260 280)
                ("L" 201 253 294)
                ("L" 205 210 231)
            )
        )
        ("R" 300 399
        )
        ("R" 600 650
            (
                 ("L" 604 605 605 606 609)
                 ("L" 643 647 649)
            )
        )
)  )  )

(main);
