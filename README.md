# HRDATA - Human Readable DATA format

### Description

Data represented by ASCII symbols, every variable is a string literal. Those string literals can be in any format and encoding and can be treated as any type.

It is possible to specify empty value by sign #. Apart from string literals and empty values, there are two data structures: map and list (or array). These data structures can be nested. Map key is always string literal. Map value and list value can be string literal, empty value (#), map or list. 

To separate entries in map and list you can use comma, however in multiline expressions, when entries separated by new line '\n', it is not required.

It is not necessary to write string literals in single quotes (') and in double quotes ("). String literal without quotes can't contain any symbols which is part of format syntax (spaces, #, {}, [], etc). String literals in single quotes and in double quotes almost the same. They can contain any symbol. To represent quotes as part of string, you should repeate them twice. The only difference between 'str' and "str" strings is that all comments inside 'str' treated as comments, but inside "str" as part of string.

Two types of comments: single line comment(starts with //), multiline coment(starts with /* and ends with */).

### More formalized description

VALUE: (starting point)
  - \# (empty value)
  - STRING
  - LIST
  - MAP
    
STRING:
  - line_without_spec_symbols
  - 'multiline with repeated '' and removed /\*comments\*/' 
  - "multiline with repeated "" and /\*comments\*/ as their part"

LIST:
  - [ ]
  - [ LIST_ENTRIES ]
    
MAP:
  - { }
  - { MAP_ENTRIES }

LIST_ENTRIES:
  - VALUE
  - VALUE DELIMITER LIST_ENTRIES

MAP_ENTRIES:
  - PAIR
  - PAIR DELIMITER MAP_ENTRIES

PAIR:
  - STRING : VALUE

DELIMITER:
  - ,
  - \n (it is possible to use as many "\n" as you want, however if "," is missed you have to place at least one "\n")

### Examples
#### String examples
```
[
    abc // abc
    'def ''g"hi' // def 'g"hi
    "abc ""d'ef" // abc "d'ef
]
```
#### Nested maps example
```
{
    m1 : { x: 10, y: 'abc', t: "def" }
    m2 : {
        x: #
        y: {
            a: y, 
            b: #,
            c: #
        }
        z: "def"
    }
}
```
#### Nested lists example
```
[
    [ 10, 'abc', "def" ]
    [
        #
        'abc'
        "def"
    ]
]
```
#### Complete example:
```
{
    m1 : [ 10, #, "def" ]
    m2 : {
        x: # // one line comment
        y: {
            a: '11
                '' // <- this is one ' sign!
                // This text will be erased
                /*
                    And this too
                */
            11'
            a2: "2345 678
                
                "" // <- this is one "" sign!
                // This text will be part of entry a2
                /*
                    And this too
                */
            91011"
            
            b: #,
            c: [ 
                x
                {1: 4, 5: 6}, y
            ]
        }
    /* multiline 
       comment */
        z: "def"
    }
}

```


