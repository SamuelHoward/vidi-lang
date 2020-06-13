ocamlopt -c tokenTypes.ml
ocamlopt -c parseTypes.ml
ocamlopt -c interpTypes.ml
ocamlopt -c lexer.ml
ocamlopt -c parser.ml
ocamlopt -c interpreter.ml
ocamlopt -c vidic.ml

ocamlopt -o vidic str.cmxa tokenTypes.cmx  parseTypes.cmx  interpTypes.cmx  lexer.cmx  parser.cmx  interpreter.cmx vidic.cmx
