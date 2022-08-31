grammar GraphUndirected;

graph: EOL* edges (EOL+ edges)* EOL* EOF;
edges: node (edge node)+;
edge: NONE_ARROW;
node: ID | STRING;

NONE_ARROW: '--';

STRING: '"' ( '\\"' | . )*? '"';
ID: LETTER+;
fragment LETTER: ~[ \t\n\r\-<>"];

EOL: '\r\n' | '\n';

// Ignored TOKENS
WS: [ \t] -> skip;
COMMENT: '/*' .*? '*/' -> skip;
LINE_COMMENT: '//' .*? '\r'? '\n' -> skip;

// vim: filetype=antlr
