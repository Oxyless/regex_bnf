#ifndef __TREE_H__
# define __TREE_H__

# include <list>
# include <string>

namespace	Bnf
{
	class	Tree
	{
	public:
		Tree(void);
		Tree(std::string);
		Tree(const Tree &);
		~Tree(void);
		const Tree &			operator=(const Tree &);		
		void					display(unsigned int offset = 0) const;
		void					add(Tree *);
		void					remove();
		Tree					*top();
		void					setToken(const std::string &);
		unsigned int			getChildListSize();
		const std::list<Tree *>	&getChildList() const;
		const std::string		&getToken() const;
	private:
		std::string			_tocken;
        std::list<Tree *>	_childList;
	};

	/*
	**	Algorithm
	*/
	class		DisplayTreeAlgorithm
	{
	public:
		DisplayTreeAlgorithm(void);
		void	operator()(const Tree *) const;
		void	setOffset(unsigned int);

	private:
		unsigned int		_offset;
	};
}

#endif // !__TREE_H__
