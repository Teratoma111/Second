#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct tree_node
{
    int value = 0;
    char* s;
    tree_node* left;
    tree_node* right;
};

tree_node* make_node(char* value, int number)
{
    tree_node* out = (tree_node *)calloc(1, sizeof(tree_node));
    out -> left = NULL;
    out -> right = NULL;
    if(value != NULL)
    {
    size_t a = strlen(value);
    out -> s = (char *) calloc(a,sizeof(char));
    for(size_t i = 0;i < a;i++)
    {
        out -> s[i] = value[i];
    }
    }
    else
    {
        out -> value = number;
    }
    return out;
}
void visitor(FILE* f,const tree_node* start)
{
    if(start -> left != NULL && start -> right != NULL)
    fprintf(f,"(");
    else
    {
    if(start -> s != NULL && (start -> s[0] == 'x' || start -> s[0] == 'c' ||start -> s[0] == 'l' || start -> s[0] == 's' ))
    fprintf(f,"(");
    }
    if(start -> left != NULL)
    {
        if(start -> s != NULL && start -> s[0] == '/')
        {
            fprintf(f,"\\frac");
            fprintf(f,"{");
            visitor(f,start -> left);
            fprintf(f,"}");
        }
        else
        visitor(f,start -> left);
    }
    if(start -> s != NULL)
    {
        if(start -> s[0] == '/')
        {
        }
        else
        fprintf(f,"%s",start ->s);
    }
        else
        {
            fprintf(f,"%d",start ->value);
        }
    if(start -> right != NULL)
    {
       if(start -> s != NULL && start -> s[0] == '/')
        {
            
            fprintf(f,"{");
            visitor(f,start -> right);
            fprintf(f,"}");
        }
        else
        visitor(f,start -> right);
    }
  if(start -> left != NULL && start -> right != NULL)
    fprintf(f,")");
    else
    {
  if(start -> s != NULL && (start -> s[0] == 'x' || start -> s[0] == 'c' ||start -> s[0] == 'l' || start -> s[0] == 's' ))
    fprintf(f,")");
    }
}
tree_node* getN(char** s)
{
     if(**s == 'x')
    {
        (*s)++;
        return make_node("x",0);
    }
    int value = 0;
    while(**s - '0' >= 0 && **s - '0' <=9)
    {
        value=value*10 +  **s - '0';
        (*s)++;
    }
    return make_node(NULL,value);
}
tree_node* getP(char** s);
tree_node* getF(char** s);
tree_node* getT(char** s)
{
    tree_node* temp = getF(s);
    tree_node* out = temp;
    if(**s == '*' || **s== '/')
    {
        if(**s == '/')
        out = make_node ("/",0);
        else
        out = make_node ("*",0);
        (*s)++;
        out->left = temp;
        out->right = getT(s);
    }
    return out;
}
tree_node* getF(char** s)
{
    tree_node* out;
    switch (**s)
    {
    case 's':
        if(*(*s+1)=='i')
        {
       out = make_node("sin",0);
       (*s)++;
       (*s)++;
       (*s)++;
       out-> right = getP(s);
        }
        else
         {
       out = make_node("sqrt",0);
       (*s)++;
       (*s)++;
       (*s)++;
       (*s)++;
       out-> right = getP(s);
        }
        break;
    case 'c':
       out = make_node("cos",0);
       (*s)++;
       (*s)++;
       (*s)++;
       out-> right = getP(s);
        break;
    case 'e':
       out = make_node("exp",0);
       (*s)++;
       (*s)++;
       (*s)++;
       out-> right = getP(s);
        break;
    case 'l':
       out = make_node("ln",0);
       (*s)++;
       (*s)++;
       out-> right = getP(s);
        break;
    default:
    out = getP(s);
        break;
    }
    return out;
}
tree_node* getE(char** s)
{
    tree_node* temp = getT(s);
    tree_node* out = temp;
    if(**s == '+' || **s== '-')
    {
        if(**s == '-')
        out = make_node ("-",0);
        else
        out = make_node ("+",0);
        (*s)++;
        out->left = temp;
        out->right = getE(s);
    }
    return out;
}
tree_node* getP(char** s)
{
    tree_node* out;
    char p=**s;
    if(**s == '(')
    {
        (*s)++;
        out = getE(s);
        (*s)++;
    }
    else
        out=getN(s);
    return out;
}
tree_node* getG(char** s)
{
    tree_node* out = getE(s);
    return out;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void delete_tree(tree_node* root)
{
    if(root != NULL)
    {
        if (root->right != NULL) {
            delete_tree(root->right);
        }
        if (root->left != NULL) {
            delete_tree(root->left);
        }
    }
    free(root -> s);
    free(root);
}
tree_node* the_differ(const tree_node* root)
{
    tree_node* out;
    if(root -> s != NULL)
    {
        char switcher = root -> s[0];
        switch (switcher)
        {
        case '+':
            out = make_node("+",0);
            out -> right = the_differ(root -> right);
            out -> left = the_differ(root -> left);
            break;
        case '-':
        out = make_node("-",0);
        out -> right = the_differ(root -> right);
        out -> left = the_differ(root -> left);
        break;
        case '*':
        out = make_node("+",0);
        out -> right = make_node("*",0);
        out -> right -> right = the_differ(root -> right);
        out -> right -> left = root -> left;
        out -> left = make_node("*",0);
        out -> left -> right = the_differ(root -> left);
        out -> left -> left = root -> right;
        break;
        case '/':
        out = make_node("/",0);
        out -> left = make_node("-",0);
        out -> left -> left = make_node("*",0);
        out -> left -> left -> left = the_differ(root -> left);
        out -> left -> left -> right = root -> right;
        out -> left -> right = make_node("*",0);
        out -> left -> right -> left = the_differ(root -> right);
        out -> left -> right -> right = root -> left;
        out -> right = make_node("*",0);
        out -> right -> left = root -> right;
        out -> right -> right = root -> right;
        break;
        case 'x':
        out = make_node(NULL,1);
        break;
        case 's':
        if(root -> s[1] == 'i')
        {
        out = make_node("*",0);
        out -> left = make_node("cos",0);
        out -> left -> right = root -> right;
        out -> right = the_differ(root -> right);
        }
        else
        {
        out = make_node("*",0);
        out -> left = make_node("/",0);
        out-> left -> left = make_node(NULL,1);
        out -> left -> right = make_node("*",0);
        out -> left -> right -> left = make_node(NULL,2);
        out -> left -> right -> right = make_node("sqrt",0);
        out -> left -> right -> right -> right = root -> right;
        out -> right = the_differ(root -> right);
        }
        break;
        case 'l':
        out = make_node("*",0);
        out -> left = make_node("/",0);
        out-> left -> left = make_node(NULL,1);
        out -> left -> right = root -> right;
        out -> right = the_differ(root -> right);
        break;
        case 'c':
        out = make_node("*",0);
        out -> left = make_node("-sin",0);
        out -> left -> right = root -> right;
        out -> right = the_differ(root -> right);
        break;
        case 'e':
        out = make_node("*",0);
        out -> left = make_node("exp",0);
        out -> left -> right = root -> right;
        out -> right = the_differ(root -> right);
        break;
        default:
            break;
        }
    }
    else
    {
        out = make_node(NULL,0);
    }
    return out;
}
void the_reducer(tree_node* root,bool* k)
{ 
    if(root -> s != NULL)
    {
    if(root -> s[0] == '*')
    {
    if(root -> left != NULL && root -> left -> value == 1)
    {
        //tree_node* temp = root -> right;
        root -> value = root -> right -> value;
        //free(root ->s);
        root -> s = root -> right -> s;
        root -> left = root -> right -> left;
        root -> right = root -> right -> right;
        //free(temp);
        *k = 1;
    }
    else
    {
    if(root -> right != NULL && root -> right -> value == 1)
    {
        //tree_node* temp = root -> left;
        root -> value = root -> left -> value;
        //free(root ->s);
        root -> s = root -> left -> s;
        root -> right = root -> left -> right;
        root -> left = root -> left -> left;
        //free(temp);
    }
    }
    if(root -> left != NULL && root -> left -> s == NULL && root -> left -> value == 0)
    {
        root -> s = NULL;
        root -> value = 0;
        root -> right = NULL;
        root -> left = NULL;
        *k = 1;
    }
    else
    {
    if(root -> right != NULL &&root -> right -> value == 0 && root -> right -> s == NULL)
    { 
        root -> s = NULL;
        root -> value = 0;
        root -> right = NULL;
        root -> left = NULL;
        //delete_tree(root -> left);
        //delete_tree(root -> right);
        *k = 1;
    }
    }
    }
    else
    {
    if(root -> s[0] == '+' || root -> s[0] == '-')
    {
        if(root -> left != NULL && root -> left -> s == NULL && root -> left -> value == 0)
    {
        if(root -> s[0] == '-' && root -> right -> s != NULL)
        {
            free(root -> left);
            root -> left = NULL;
            *k = 1;
        }
        else
        {
        if(root -> s[0] == '-')
        root -> value = root -> right -> value * (-1);
        else
        root -> value = root -> right -> value;
        //tree_node* temp = root -> right;
        //free(root ->s);
        root -> s = root -> right -> s;
        root -> left = root -> right -> left;
        root -> right = root -> right -> right;
        //free(temp);
        *k = 1;
        }
    }
    else
    {
    if(root -> right != NULL && root -> right -> s == NULL && root -> right -> value == 0)
    {
        if(root -> s[0] == '-' && root -> left -> s != NULL)
        {
                free(root -> right);
                root -> right = NULL;
                *k = 1;
        }
        else
        {
        if(root -> s[0] == '-')
        root -> value = root -> left -> value * (-1);
        else
        root -> value = root -> left -> value;
        //tree_node* temp = root -> left;
        //free(root ->s);
        root -> s = root -> left -> s;
        root -> right = root -> left -> right;
        root -> left = root -> left -> left;
        //free(temp);
        *k = 1;
        }
    }
    }
    }
    }
    }
    if(root -> right != NULL)
    {
        the_reducer(root -> right,k);
    }
     if(root -> left != NULL)
    {
        the_reducer(root -> left,k);
    }
}
void true_reduce(tree_node* root)
{
    bool* k;
    k = (bool *)calloc(1, sizeof(bool));
    *k = 1;
    while(*k == 1)
    {
        *k = 0;
        the_reducer(root,k);
    }
    free(k);
}
void dump_me_pls_latex(char* s)
{
    FILE* fp;
    fp = fopen("diff.tex","w+");
    fprintf(fp,"\\documentclass[20pt,a4paper]{scrartcl}\n\\usepackage[utf8]{inputenc}\n\\usepackage[english,russian]{babel}");
    fprintf(fp,"\n\\usepackage{indentfirst}\n\\usepackage{misccorr}\n\\usepackage{graphicx}\n\\usepackage{amsmath}\n\\begin{document}\n\\begin{math}\n");
    tree_node* root = getG(&s);
    fprintf(fp,"$Сама функция:$\\\\\n");
    visitor(fp,root);
    fprintf(fp,"\\\\");
    tree_node* res = the_differ(root);
    fprintf(fp,"$Не упрощенная производная:$\\\\\n");
    visitor(fp,res);
    fprintf(fp,"\\\\");
    true_reduce(res);
    fprintf(fp,"$Итоговый ответ:$\\\\\n");
    visitor(fp,res);
    fprintf(fp,"\n\\end{math}\n\\end{document}");
    delete_tree(root);
    delete_tree(res);
}
int main() {
    char* s = (char *)calloc(30, sizeof(char));
    scanf("%s",s);
    dump_me_pls_latex(s);
    free(s);
   //2*3*x+1*x
   //3*sin(ln(1/(sqrt(x))))
    return 0;
}