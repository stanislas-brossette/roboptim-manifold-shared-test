#ifndef ROBOPTIM_SHARED_TESTS_SERIALIZE_HH
# define ROBOPTIM_SHARED_TESTS_SERIALIZE_HH

# include <Eigen/Sparse>
# include <Eigen/Dense>
# include <boost/serialization/split_free.hpp>
# include <boost/serialization/vector.hpp>

// Obtained from: https://gist.github.com/mtao/5798888
namespace boost {
  namespace serialization {
    template <class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    void save (Archive & ar, const Eigen::Matrix<_Scalar,_Rows,_Cols,_Options,_MaxRows,_MaxCols> & m, const unsigned int /*version*/)
    {
      int rows = static_cast<int> (m.rows ()), cols = static_cast<int> (m.cols ());
      ar & rows;
      ar & cols;
      ar & boost::serialization::make_array (m.data (), static_cast<std::size_t> (rows * cols));
    }

    template <class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    void load (Archive & ar, Eigen::Matrix<_Scalar,_Rows,_Cols,_Options,_MaxRows,_MaxCols> & m, const unsigned int /*version*/)
    {
      int rows,cols;
      ar & rows;
      ar & cols;
      m.resize (rows, cols);
      ar & boost::serialization::make_array (m.data (), static_cast<std::size_t> (rows * cols));
    }

    template <class Archive, typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    void serialize (Archive & ar, Eigen::Matrix<_Scalar,_Rows,_Cols,_Options,_MaxRows,_MaxCols> & m, const unsigned int version)
    {
      split_free (ar,m,version);
    }

    template <class Archive, typename _Scalar>
    void save (Archive & ar, const Eigen::Triplet<_Scalar> & m, const unsigned int /*version*/)
    {
      ar & m.row ();
      ar & m.col ();
      ar & m.value ();
    }

    template <class Archive, typename _Scalar>
    void load (Archive & ar, Eigen::Triplet<_Scalar> & m, const unsigned int /*version*/)
    {
      int row,col;
      _Scalar value;
      ar & row;
      ar & col;
      ar & value;
      m = Eigen::Triplet<_Scalar> (row, col, value);
    }

    template <class Archive, typename _Scalar>
    void serialize (Archive & ar, Eigen::Triplet<_Scalar> & m, const unsigned int version)
    {
      split_free (ar,m,version);
    }

    template <class Archive, typename _Scalar, int _Options,typename _Index>
    void save (Archive & ar, const Eigen::SparseMatrix<_Scalar,_Options,_Index> & m, const unsigned int /*version*/)
    {
      int innerSize = m.innerSize ();
      int outerSize = m.outerSize ();
      typedef typename Eigen::Triplet<_Scalar> Triplet;
      std::vector<Triplet> triplets;
      for (int i = 0; i < outerSize; ++i)
	{
	  for (typename Eigen::SparseMatrix<_Scalar,_Options,_Index>::InnerIterator it (m,i); it; ++it)
	    {
	      triplets.push_back (Triplet (it.row (), it.col (), it.value ()));
	    }
	}
      ar & innerSize;
      ar & outerSize;
      ar & triplets;
    }

    template <class Archive, typename _Scalar, int _Options, typename _Index>
    void load (Archive & ar, Eigen::SparseMatrix<_Scalar,_Options,_Index>  & m, const unsigned int /*version*/)
    {
      int innerSize;
      int outerSize;
      ar & innerSize;
      ar & outerSize;
      int rows = m.IsRowMajor?outerSize:innerSize;
      int cols = m.IsRowMajor?innerSize:outerSize;
      m.resize (rows,cols);
      typedef typename Eigen::Triplet<_Scalar> Triplet;
      std::vector<Triplet> triplets;
      ar & triplets;
      m.setFromTriplets (triplets.begin (), triplets.end ());
    }

    template <class Archive, typename _Scalar, int _Options, typename _Index>
    void serialize (Archive & ar, Eigen::SparseMatrix<_Scalar,_Options,_Index> & m, const unsigned int version)
    {
      split_free (ar,m,version);
    }
  }
}

#endif //! ROBOPTIM_SHARED_TESTS_SERIALIZE_HH
