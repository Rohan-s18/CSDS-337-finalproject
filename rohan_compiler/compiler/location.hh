/*
Author: Rohan Singh, Benjamin Luo and William Cankar
QBASIC Abstruct Syntax Tree
*/

#ifndef YY_YY_HOME_CAI_PROJECTS_BASIC_COMPILER_LOCATION_HH_INCLUDED
# define YY_YY_HOME_CAI_PROJECTS_BASIC_COMPILER_LOCATION_HH_INCLUDED

# include "position.hh"

/* Line 166 of location.cc  */
#line 21 "parser.ypp"
namespace qb {
/* Line 166 of location.cc  */
#line 48 "/home/cai/projects/basic/compiler/location.hh"

  /// Abstract a location.
  class location
  {
  public:

    /// Construct a location from \a b to \a e.
    location (const position& b, const position& e)
      : begin (b)
      , end (e)
    {
    }

    /// Construct a 0-width location in \a p.
    explicit location (const position& p = position ())
      : begin (p)
      , end (p)
    {
    }

    /// Construct a 0-width location in \a f, \a l, \a c.
    explicit location (std::string* f,
                       unsigned int l = 1u,
                       unsigned int c = 1u)
      : begin (f, l, c)
      , end (f, l, c)
    {
    }


    /// Initialization.
    void initialize (std::string* f = YY_NULL,
                     unsigned int l = 1u,
                     unsigned int c = 1u)
    {
      begin.initialize (f, l, c);
      end = begin;
    }

    /** \name Line and Column related manipulators
     ** \{ */
  public:
    /// Reset initial location to final location.
    void step ()
    {
      begin = end;
    }

    /// Extend the current location to the COUNT next columns.
    void columns (unsigned int count = 1)
    {
      end += count;
    }

    /// Extend the current location to the COUNT next lines.
    void lines (unsigned int count = 1)
    {
      end.lines (count);
    }
    /** \} */


  public:
    /// Beginning of the located region.
    position begin;
    /// End of the located region.
    position end;
  };

  /// Join two location objects to create a location.
  inline const location operator+ (const location& begin, const location& end)
  {
    location res = begin;
    res.end = end.end;
    return res;
  }

  /// Add two location objects.
  inline const location operator+ (const location& begin, unsigned int width)
  {
    location res = begin;
    res.columns (width);
    return res;
  }

  /// Add and assign a location.
  inline location& operator+= (location& res, unsigned int width)
  {
    res.columns (width);
    return res;
  }

  /// Compare two location objects.
  inline bool
  operator== (const location& loc1, const location& loc2)
  {
    return loc1.begin == loc2.begin && loc1.end == loc2.end;
  }

  /// Compare two location objects.
  inline bool
  operator!= (const location& loc1, const location& loc2)
  {
    return !(loc1 == loc2);
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param loc a reference to the location to redirect
   **
   ** Avoid duplicate information.
   */
  template <typename YYChar>
  inline std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const location& loc)
  {
    position last = loc.end - 1;
    ostr << loc.begin;
    if (last.filename
	&& (!loc.begin.filename
	    || *loc.begin.filename != *last.filename))
      ostr << '-' << last;
    else if (loc.begin.line != last.line)
      ostr << '-' << last.line  << '.' << last.column;
    else if (loc.begin.column != last.column)
      ostr << '-' << last.column;
    return ostr;
  }

/* Line 296 of location.cc  */
#line 21 "parser.ypp"
} // qb
/* Line 296 of location.cc  */
#line 182 "/home/cai/projects/basic/compiler/location.hh"

#endif /* !YY_YY_HOME_CAI_PROJECTS_BASIC_COMPILER_LOCATION_HH_INCLUDED  */
