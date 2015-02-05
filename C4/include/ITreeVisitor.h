/*
======================================
	Interface to visit parse trees
	and perform certain operations
	on them.
======================================
*/

#ifndef __AST_VISITOR_INTERFACE_H__
#define __AST_VISITOR_INTERFACE_H__

namespace c4 {

class CompilationUnit;

class Declaration;
class DeclarationList;
class VarDecl;
class Argument;
class ArgumentList;
class FunctionDecl;
class ClassDecl;
class StructDecl;
class EnumDecl;
class NamespaceDecl;

class Statement;
class EmptyStatement;
class ErrorStatement;
class StatementList;
class ExpressionStatement;
class LabeledStatement;
class DeclStatement;
class Block;
class AutoDeclStatement;
class IfStatement;
class LoopStatement;
class WhileStatement;
class DoStatement;
class WithStatement;
class ReturnStatement;
class ForLoopStatement;
class CaseStatement;
class DefaultStatement;
class ContinueStatement;
class BreakStatement;

class Expression;
class ExpressionList;
class BooleanConstant;
class IntegerConstant;
class FloatingConstant;
class CharConstant;
class StringLiteral;
class IdExp;
class VarExp;
class Identifier;
class UnaryExpression;
class BinaryExpression;
class AssertExpression;
class AddrOfExp;
class ArrayAccessExp;
class PtrIndirectionExp;
class DotExp;
class CastExp;
class NegationExp;
class UnaryPlusExp;
class PreIncExp;
class PreDecExp;
class PostIncExp;
class PostDecExp;
class ComplExp;
class NotExp;
class CommaExpression;
class AssignmentExp;
class AddExp;
class SubExp;
class MulExp;
class DivExp;
class ModExp;
class ShlExp;
class ShrExp;
class UShrExp;
class XorExp;
class OrOrExp;
class InExp;
class CondExp;
class EquExp;
class RelExp;
class BitAndExp;
class BitXorExp;
class BitOrExp;
class AddAssignExp;
class SubAssignExp;
class MulAssignExp;
class DivAssignExp;
class ModAssignExp;
class BitAndAssignExp;
class BitXorAssignExp;
class BitOrAssignExp;
class ShlAssignExp;
class ShrAssignExp;
class UShrAssignExp;
class CallExp;
class ErroneousExpression;
class ThisExpression;

class AST_Node;

//==========================================================================

// this macro must be included in a visitable class's declaration

#define VISITABLE()	\
	public:			\
	virtual void	Accept( ITreeVisitor * v ) { v->Visit( this ); }


//==========================================================================

class ITreeVisitor {
public:
	virtual			~ITreeVisitor() {}

	virtual void	Visit( CompilationUnit *		compUnit 		) = 0;

	virtual void	Visit( Declaration *			decl 			) = 0;
	virtual void	Visit( DeclarationList *		declList 		) = 0;
	virtual void	Visit( VarDecl * 				varDecl 		) = 0;
	virtual void	Visit( Argument *				arg 			) = 0;
	virtual void	Visit( ArgumentList * 			argList 		) = 0;
	virtual void	Visit( FunctionDecl * 			functionDecl 	) = 0;
	virtual void	Visit( ClassDecl * 				classDecl 		) = 0;
	virtual void	Visit( StructDecl *				structDecl 		) = 0;
	virtual void	Visit( EnumDecl *				enumDecl 		) = 0;
	virtual void	Visit( NamespaceDecl * 			namespaceDecl 	) = 0;

	virtual void	Visit( Statement *				stmt 			) = 0;
	virtual void	Visit( EmptyStatement *			emptyStmt 		) = 0;
	virtual void	Visit( ErrorStatement *			errorStmt 		) = 0;
	virtual void	Visit( StatementList *			stmtList 		) = 0;
	virtual void	Visit( ExpressionStatement *	expStmt 		) = 0;
	virtual void	Visit( LabeledStatement *		labelStmt 		) = 0;
	virtual void	Visit( DeclStatement *			declStmt 		) = 0;
	virtual void	Visit( Block *					block 			) = 0;
	virtual void	Visit( AutoDeclStatement *		autoDeclStmt 	) = 0;
	virtual void	Visit( IfStatement *			ifStmt 			) = 0;
	virtual void	Visit( LoopStatement *			loopStmt 		) = 0;
	virtual void	Visit( WhileStatement *			whileStmt 		) = 0;
	virtual void	Visit( DoStatement *			doStmt 			) = 0;
	virtual void	Visit( WithStatement *			withStmt 		) = 0;
	virtual void	Visit( ReturnStatement *		returnStmt 		) = 0;
	virtual void	Visit( ForLoopStatement *		forStmt			) = 0;
	virtual void	Visit( CaseStatement *			caseStmt 		) = 0;
	virtual void	Visit( DefaultStatement *		defaultStmt 	) = 0;
	virtual void	Visit( ContinueStatement *		continueStmt 	) = 0;
	virtual void	Visit( BreakStatement *			breakStmt 		) = 0;
	
	virtual void	Visit( Expression *				expr 			) = 0;
	virtual void	Visit( ExpressionList *			exprList 		) = 0;
	virtual void	Visit( BooleanConstant * 		boolConst		) = 0;
	virtual void	Visit( IntegerConstant * 		intConst		) = 0;
	virtual void	Visit( FloatingConstant *		floatConst		) = 0;
	virtual void	Visit( CharConstant *			charConst 		) = 0;
	virtual void	Visit( StringLiteral *			strConst 		) = 0;	
	virtual void	Visit( IdExp *					idExp			) = 0;
	virtual void	Visit( VarExp *					varExp			) = 0;
	virtual void	Visit( Identifier * 			id				) = 0;
	virtual void	Visit( UnaryExpression *		unaryExp		) = 0;
	virtual void	Visit( BinaryExpression * 		binaryExp		) = 0;
	virtual void	Visit( AssertExpression * 		assertExp 		) = 0;
	virtual void	Visit( AddrOfExp *				addrOfExpr		) = 0;
	virtual void	Visit( ArrayAccessExp *			arrayExp		) = 0;
	virtual void	Visit( PtrIndirectionExp *		ptrExp			) = 0;
	virtual void	Visit( DotExp *					dotExp			) = 0;
	virtual void	Visit( CastExp *				castExp			) = 0;
	virtual void	Visit( NegationExp * 			negExp			) = 0;
	virtual void	Visit( UnaryPlusExp *			uplusExp		) = 0;
	virtual void	Visit( PreIncExp * 				preIncExp		) = 0;
	virtual void	Visit( PreDecExp * 				preDecExp		) = 0;
	virtual void	Visit( PostIncExp * 			postIncExp		) = 0;
	virtual void	Visit( PostDecExp * 			postDecExp		) = 0;
	virtual void	Visit( ComplExp *				complExp		) = 0;
	virtual void	Visit( NotExp *					notExp			) = 0;
	virtual void	Visit( CommaExpression *		commaExp		) = 0;
	virtual void	Visit( AssignmentExp *			assignExp		) = 0;
	virtual void	Visit( AddExp * 				addExp			) = 0;
	virtual void	Visit( SubExp * 				subExp			) = 0;
	virtual void	Visit( MulExp * 				mulExp			) = 0;
	virtual void	Visit( DivExp * 				divExp			) = 0;
	virtual void	Visit( ModExp * 				modExp			) = 0;
	virtual void	Visit( ShlExp * 				shlExp			) = 0;
	virtual void	Visit( ShrExp * 				shrExp			) = 0;
	virtual void	Visit( UShrExp *				ushrExp			) = 0;
	virtual void	Visit( XorExp *					xorExp			) = 0;
	virtual void	Visit( OrOrExp *				orExp			) = 0;
	virtual void	Visit( InExp * 					inExp			) = 0;
	virtual void	Visit( CondExp * 				condExp			) = 0;
	virtual void	Visit( EquExp * 				equExp			) = 0;
	virtual void	Visit( RelExp * 				relExp			) = 0;
	virtual void	Visit( BitAndExp * 				bitAndExp		) = 0;
	virtual void	Visit( BitXorExp * 				bitXorExp		) = 0;
	virtual void	Visit( BitOrExp * 				bitOrExp		) = 0;
	virtual void	Visit( AddAssignExp * 			addAssignExp	) = 0;
	virtual void	Visit( SubAssignExp * 			subAssignExp	) = 0;
	virtual void	Visit( MulAssignExp * 			mulAssignExp	) = 0;
	virtual void	Visit( DivAssignExp * 			divAssign		) = 0;
	virtual void	Visit( ModAssignExp * 			modAssign		) = 0;
	virtual void	Visit( BitAndAssignExp * 		bitAndAssignExp	) = 0;
	virtual void	Visit( BitXorAssignExp * 		bitXorAssignExp	) = 0;
	virtual void	Visit( BitOrAssignExp * 		bitOrAssignExp	) = 0;
	virtual void	Visit( ShlAssignExp * 			shlAssignExp	) = 0;
	virtual void	Visit( ShrAssignExp * 			shrAssignExp	) = 0;
	virtual void	Visit( UShrAssignExp * 			ushrAssignExp	) = 0;
	virtual void	Visit( CallExp *				callExp			) = 0;
	virtual void	Visit( ErroneousExpression *	errorExp		) = 0;
	virtual void	Visit( ThisExpression *			thisExp			) = 0;

	virtual void	Visit( AST_Node *				node			) = 0;
};

}//end namespace c4

#endif // !__AST_VISITOR_INTERFACE_H__