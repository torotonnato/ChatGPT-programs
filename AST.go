//Write a golang function that accept a string s as a parameter. The string has this grammar:
//
//program := (stmt delim)+
//delim := ';' '\n+'
//stmt := op reg reg
//op := 'add' | 'sub' | 'mul' | 'div'
//reg := 'ax' | 'bx' | 'cx' | 'dx'
//
//The output of the function is an AST

package main

import (
    "fmt"
    "strings"
)

// ASTNode represents a node in the abstract syntax tree.
type ASTNode struct {
    op string
    left, right string
}

// parseProgram parses a string according to the grammar specified in the prompt
// and returns the corresponding abstract syntax tree.
func parseProgram(s string) ([]ASTNode, error) {
    // Split the input string into individual statements.
    stmts := strings.Split(s, ";")

    // Parse each statement and add it to the AST.
    var ast []ASTNode
    for _, stmt := range stmts {
        // Parse the statement and add it to the AST.
        node, err := parseStmt(stmt)
        if err != nil {
            return nil, err
        }
        ast = append(ast, node)
    }

    return ast, nil
}

// parseStmt parses a string according to the grammar specified in the prompt
// and returns the corresponding AST node.
func parseStmt(s string) (ASTNode, error) {
    // Split the input string into individual words.
    words := strings.Fields(s)

    // Check that the statement has the correct number of words.
    if len(words) != 3 {
        return ASTNode{}, fmt.Errorf("expected 3 words, got %d", len(words))
    }

    // Parse the operation and operand registers.
    var node ASTNode
    node.op = words[0]
    node.left = words[1]
    node.right = words[2]

    return node, nil
}

func main() {
    // Parse a program and print the resulting AST.
    ast, err := parseProgram("add ax bx; sub cx dx")
    if err != nil {
        fmt.Println(err)
    } else {
        fmt.Println(ast)
    }
}
