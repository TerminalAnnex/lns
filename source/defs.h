//
// Created by lorenzo on 31/08/17.
//

#ifndef CPPLNS_DEFS_H
#define CPPLNS_DEFS_H

#include <sstream>
#include <string>
#include <string>
#include <map>
#include <cmath>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <exception>
#include "cstring"
#include <memory>

#define NATIVE_RUNTIME_EXC(msg) throw runtime_exception(__FILE__,__LINE__,*new std::string(msg));
#define STR(S) *new string(S)
#define WRONG_OP(OP) throw lns::INVALID_OP(#OP,this->type,&o.type);
#define WRONG_OP_UN(OP) throw lns::INVALID_OP(#OP,this->type,nullptr);

#define DCAST(a,b) (dynamic_cast<a>(b))
#define DCAST_ASN(a,b,c) a = DCAST(b,c)
#define DCAST_ASNCHK(a,b,c) ((DCAST_ASN(a,b,c)) != nullptr)
#define DCAST_CHK(b,c) (DCAST(b,c) != nullptr)

#define LNS_INSTALL_LOCATION "/lns/"
#define LNS_LIBRARY_LOCATION LNS_INSTALL_LOCATION "lib/"

#define DEBUGGER_WATCHES_FILENAME "..##DEBUG_WATCH##.."

#define CHARSTR(c) string(1,c)

#define CODE_LEAK_ERROR (-250)


namespace lns {
    class parameter_declaration;

    const char* best_file_path(const char* filename);
    enum token_type {
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_SQR,
        RIGHT_SQR,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        DOT,
        COLON,
        MINUS,
        MINUS_EQUALS,
        PLUS,
        PLUS_EQUALS,
        PLUS_PLUS,
        MINUS_MINUS,
        SLASH,
        SLASH_EQUALS,
        STAR,
        STAR_EQUALS,
        HAT,
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,
        IDENTIFIER,
        STRING,
        NUMBER,
        NATIVES,
        AND,
        CLASS,
        ELSE,
        FALSE,
        FUNCTION,
        FOR,
        FOREACH,
        IF,
        NUL,
        OR,
        XOR,
        NOR,
        NAND,
        NOT,
        RETURN,
        SUPER,
        THIS,
        TRUE,
        VAR,
        WHILE,
        GLOBAL,
        LOCAL,
        FINAL,
        USE,
        BREAK,
        CONTINUE,
        CONTEXT,
        BEGIN,
        THEN,
        DO,
        END,
        DPCHECK,
        IN,
        BIND,
        EXCEPTION,
        WITH,
        RAISE,
        HANDLE,
        CONSTRUCTOR,
        UNRECOGNIZED,
        QUESTION_MARK,
        NEW,
        EXTENDS,
        EOF_
    };

    enum objtype {
        NUMBER_T, STRING_T, BOOL_T, NULL_T, ARRAY_T, CALLABLE_T, NATIVE_CALLABLE_T, CONTEXT_T, EXCEPTION_T,
        EXCEPTION_DEFINITION_T, CLASS_DEFINITION_T, INSTANCE_T, UNSPECIFIED
    };

    enum operator_location{
        POSTFIX, PREFIX
    };

    enum visibility{
        V_GLOBAL, V_LOCAL, V_UNSPECIFIED
    };

    typedef objtype object_type;


    const std::string KEYWORDS_S_VALUES[] = {"COMMA", "LEFT_PAREN", "RIGHT_PAREN", "LEFT_SQR", "RIGHT_SQR", "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT", "COLON", "MINUS",
                                             "MINUS_EQUALS", "PLUS", "PLUS_EQUALS", "PLUS_PLUS", "MINUS_MINUS",
                                             "SLASH", "SLASH_EQUALS", "STAR", "STAR_EQUALS", "HAT", "BANG", "BANG_EQUAL",
                                             "EQUAL", "EQUAL_EQUAL", "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL",
                                             "IDENTIFIER", "STRING", "NUMBER", "NATIVES", "AND", "CLASS", "ELSE", "FALSE",
                                             "FUNCTION", "FOR", "FOREACH", "IF", "NUL", "OR", "XOR", "NOR", "NAND", "NOT", "RETURN",
                                             "SUPER", "THIS", "TRUE", "VAR", "WHILE", "GLOBAL", "LOCAL", "FINAL", "USE", "BREAK",
                                             "CONTINUE", "CONTEXT", "BEGIN", "THEN", "DO", "END", "DPCHECK", "IN", "BIND", "EXCEPTION", "WITH", "RAISE", "HANDLE", "CONSTRUCTOR", "UNRECOGNIZED", "QUESTION_MARK",
                                             "NEW", "EXTENDS", "EOF_"};

    const char* INVALID_OP(const char* OP, const lns::object_type t1, const lns::object_type* t2);

    class object {
    private:
        object();

    public:
        objtype type;

        explicit object(objtype t);

        virtual std::string str() const = 0;

        virtual bool operator==(const object &o) const { WRONG_OP(==) };

        virtual bool operator&&(const object &o) const { WRONG_OP(and) };

        virtual bool operator||(const object &o) const { WRONG_OP(or) };

        virtual std::shared_ptr<object> operator!() const { WRONG_OP_UN(not) };

        virtual bool operator>(const object &o) const { WRONG_OP(>) };

        virtual bool operator>=(const object &o) const{ WRONG_OP(>=) };

        virtual bool operator<=(const object &o) const{ WRONG_OP(<=) };

        virtual bool operator<(const object &o) const{ WRONG_OP(<) };

        virtual void operator+=(const object &o) { WRONG_OP(+=) };

        virtual void operator-=(const object &o) { WRONG_OP(-=) };

        virtual void operator*=(const object &o) { WRONG_OP(*=) };

        virtual void operator/=(const object &o) { WRONG_OP(/=) };

        virtual std::shared_ptr<object> operator+(const object &o) const { WRONG_OP(+) };

        virtual std::shared_ptr<object> operator-(const object &o) const { WRONG_OP(-) };

        virtual std::shared_ptr<object> operator*(const object &o) const { WRONG_OP(*) };

        virtual std::shared_ptr<object> operator/(const object &o) const { WRONG_OP(/) };

        virtual std::shared_ptr<object> operator^(const object &o) const { WRONG_OP(^) };

        virtual std::shared_ptr<object> operator-() const { WRONG_OP_UN(-) };

        virtual std::shared_ptr<object> operator++() { WRONG_OP_UN(++) };

        virtual std::shared_ptr<object> operator--() { WRONG_OP_UN(--) };

        virtual std::shared_ptr<object> operator++(int) { WRONG_OP_UN(++) };

        virtual std::shared_ptr<object> operator--(int) { WRONG_OP_UN(--) };

        virtual std::shared_ptr<object> clone() const = 0;
    };

    bool check_type(object_type type, const object &o1, const object &o);

    class string_o : public object {
    public:
        std::string value;

        std::shared_ptr<object> clone() const override;

        explicit string_o(std::string value);

        string_o();

        std::string str() const override;

        bool operator==(const object &o) const override;

        void operator+=(const object &o) override;

        bool operator>(const object &o) const override;

        bool operator>=(const object &o) const override;

        bool operator<=(const object &o) const override;

        bool operator<(const object &o) const override;

        std::shared_ptr<object> operator+(const object &o) const override;

        virtual std::shared_ptr<object> operator/(const object &o) const;
    };

    class number_o : public object {
    public:
        long double value;

        explicit number_o(double value);

        std::string str() const override;

        bool operator==(const object &o) const override;

        bool operator>(const object &o) const override;

        bool operator>=(const object &o) const override;

        bool operator<=(const object &o) const override;

        bool operator<(const object &o) const override;

        void operator+=(const object &o) override;

        void operator-=(const object &o) override;

        void operator*=(const object &o) override;

        void operator/=(const object &o) override;

        std::shared_ptr<object> operator+(const object &o) const override;

        std::shared_ptr<object> operator-(const object &o) const override;

        std::shared_ptr<object> operator*(const object &o) const override;

        std::shared_ptr<object> operator/(const object &o) const override;

        std::shared_ptr<object> operator^(const object &o) const override;

        std::shared_ptr<object> operator-() const override;

        std::shared_ptr<object> operator++() override;

        std::shared_ptr<object> operator--() override;

        std::shared_ptr<object> operator++(int) override;

        std::shared_ptr<object> operator--(int) override;

        std::shared_ptr<object> clone() const;
    };

    class bool_o : public object {
    public:
        bool value;

        explicit bool_o(bool value);

        std::string str() const override {
            return value ? "true" : "false";
        }

        bool operator==(const object &o) const override;

        bool operator&&(const object &o) const override;

        bool operator||(const object &o) const override;

        std::shared_ptr<object> operator!() const override;

        std::shared_ptr<object> operator+(const object &o) const override;

        std::shared_ptr<object> clone() const override;
    };

    class null_o : public object {
    public:
        explicit null_o();

        std::string str() const override;

        bool operator==(const object &o) const override;

        std::shared_ptr<object> clone() const override;
    };

    class array_o : public object {
    private:
    public:
        explicit array_o();

        std::map<double, std::shared_ptr<object>> values;

        bool operator==(const object &o) const override;

        std::string str() const override;

        const bool contains_key(double t);

        std::shared_ptr<object> clone() const;

        std::shared_ptr<object> operator+(const object &o) const override;
    };

    class variable {
    public:
        const bool is_final;
        const visibility visibility_;
        const char *def_file;
        std::shared_ptr<object> value;

        explicit variable();

        variable(lns::visibility vis, bool is_final, std::shared_ptr<object> value, const char *def_file);

        const variable& operator=(variable const &v);
    };

    class token {
    public:
        token(token_type type, std::string lexeme, std::shared_ptr<object> literal, const char *filename, int line);

        token_type type;
        std::string lexeme;
        std::shared_ptr<object> literal;
        const char *filename;
        int line;

        std::string format() const;
        //void operator delete(void*){}
    };


    class callable : public object{
    public:
        callable() = delete;
        callable(bool is_native);
        virtual const parameter_declaration& arity() const = 0;

        virtual const std::string name() const = 0;

        virtual std::shared_ptr<object> call(std::vector<std::shared_ptr<object>> &args) = 0;
        bool operator==(const object &o) const override;
        std::string str() const override;

        std::shared_ptr<object> clone() const override;

    };

    class function_container : public object{
    public:
        explicit function_container(objtype type);

        virtual std::set<callable *> declare_natives() const = 0;
    };

    class native_callable : public callable{
    private:
        parameter_declaration& parameters;
    public:
        native_callable(int arity);
        const parameter_declaration& arity() const;
    };

    class stack_call {
    public:
        const char *filename;
        const int line;
        const std::string method;
        const bool native;

        stack_call(const char *filename, const int line, const std::string method, const bool native);
    };

    class runtime_environment {
    protected:
        std::set <callable*> natives;
        std::shared_ptr<runtime_environment> enclosing;
        std::map<std::string, lns::variable> values;

        bool contains_key(std::string name);

    public:
        explicit runtime_environment();

        //virtual ~runtime_environment();

        explicit runtime_environment(std::shared_ptr<runtime_environment> enc);

        std::shared_ptr<object> get(const token *name, const char *accessing_file);

        virtual void define(const token *name, std::shared_ptr<object> o, bool is_final, visibility visibility,
                            const char *def_file);

        virtual void
        define(const std::string &name, std::shared_ptr<object> o, bool is_final, visibility vis, const char *def_file);

        void assign(const token *name, token_type op, std::shared_ptr<object> obj, const char *assigning_file);


        bool clear_var(const token *name);

        bool is_native(callable *ptr);

        void reset();

        void add_natives(const std::set<callable*>& natives);

        void add_native(callable *ptr);
    };

    class context : public lns::runtime_environment, public function_container {
    public:
        explicit context(std::shared_ptr<runtime_environment> previous) : runtime_environment(previous),
                                                                          function_container(CONTEXT_T) {}

        void define(const std::string &name, std::shared_ptr<object> o, bool is_final, visibility vis,
                    const char *def_file) override;

        std::string str() const override;

        bool operator==(const object &o) const override;

        std::shared_ptr<object> clone() const override;

        std::set<callable *> declare_natives() const override;

        void define(const token *name, std::shared_ptr<object> o, bool is_final, visibility visibility,
                    const char *def_file) override;
    };



    class return_signal : public std::exception {
    public:
        std::shared_ptr<lns::object> value;
        const token *keyword;

        return_signal(std::shared_ptr<object> value, const token *keyword);

        const char *what() const throw() override;
    };

    class break_signal : public std::exception {
    public:
        const token *keyword;

        explicit break_signal(const token *keyword);

        const char *what() const throw() override;
    };

    class continue_signal : public std::exception {
    public:
        const token *keyword;

        const char *what() const throw() override;

        explicit continue_signal(const token *keyword);
    };

    std::shared_ptr<object> GET_DEFAULT_NULL();

    inline bool check_type(object_type type, const object &o1, const object &o) {
        return (o1.type == type && o.type == type);
    }

    class runtime_exception : public std::exception {
    public:
        std::string message;
        bool native_throw;
        const lns::token *token;

        runtime_exception(const char *filename, int line, std::string message);

        runtime_exception(const lns::token *token, std::string m);

        const char *what() const throw() override;
    };

    class incode_exception : public lns::runtime_exception, public lns::object{
    private:
        std::map<std::string, std::shared_ptr<object> > fields;
    public:
        int calls_bypassed;

        incode_exception(const lns::token *token, std::string m,
                         const std::map<std::string, std::shared_ptr<object>> fields);

        std::shared_ptr<object> get(std::string &key);

        bool operator==(const object &o) const override;

        std::string str() const override;

        std::shared_ptr<object> clone() const override;

        const char *what() const throw() override;
    };

    const char *type_to_string(object_type t);

    class exception_definition : public object{
    public:
        const std::string &name;
        const std::set<std::string>& fields;
        const char* def_file;
        int def_line;
        exception_definition(const char* def_file, int def_line, const std::string& name, const std::set<std::string>& fields);

        std::string str() const override;

        bool operator==(const object &o) const override;

        std::shared_ptr<object> clone() const override;
    };

    const char *concat(std::initializer_list<std::__cxx11::string> ss);


    class parameter{
    public:
        std::string& name;
        bool nullable;
        parameter(std::string& name, bool nullable);
    };

    class parameter_declaration{
    public:
        parameter_declaration(int i);

        std::vector<parameter>& parameters;
        parameter_declaration();
        explicit parameter_declaration(std::vector<parameter>& parameters);
        const int required() const;
        const int optional() const;
    };

    inline bool file_exists(const char* name);
}

#endif //CPPLNS_DEFS_H