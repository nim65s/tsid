//
// Copyright (c) 2017 CNRS
//
// This file is part of PinInvDyn
// pinocchio is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
// pinocchio is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// pinocchio If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef __invdyn_solvers_hqp_eiquadprog_hpp__
#define __invdyn_solvers_hqp_eiquadprog_hpp__

#include <pininvdyn/solvers/solver-HQP-base.h>

#define DEFAULT_HESSIAN_REGULARIZATION 1e-8

namespace pininvdyn
{
  namespace solvers
  {
    /**
     * @brief Abstract interface for a Quadratic Program (HQP) solver.
     */
    class PININVDYN_DLLAPI Solver_HQP_eiquadprog:
        public Solver_HQP_base
    {
    public:
      typedef pininvdyn::math::Matrix Matrix;
      typedef pininvdyn::math::Vector Vector;
      typedef pininvdyn::math::RefVector RefVector;
      typedef pininvdyn::math::ConstRefVector ConstRefVector;
      typedef pininvdyn::math::ConstRefMatrix ConstRefMatrix;

      Solver_HQP_eiquadprog(const std::string & name);

      void resize(unsigned int n, unsigned int neq, unsigned int nin);

      /** Solve the given Hierarchical Quadratic Program
       */
      const HqpOutput & solve(const HqpData & problemData);

      /** Get the objective value of the last solved problem. */
      double getObjectiveValue();

    protected:

      void sendMsg(const std::string & s);

      Matrix m_H;
      Vector m_g;
      Matrix m_CE;
      Vector m_ce0;
      Matrix m_CI;
      Vector m_ci0;
      double m_objValue;

      double m_hessian_regularization;

      Eigen::VectorXi m_activeSet;  /// vector containing the indexes of the active inequalities
      int m_activeSetSize;

#ifdef ELIMINATE_EQUALITY_CONSTRAINTS
//      Eigen::FullPivLU<Matrix>                        m_CE_dec;
//	  Eigen::ColPivHouseholderQR<Matrix>              m_CE_dec; // fast, but difficult to retrieve null space basis
//      Eigen::FullPivHouseholderQR<Matrix>             m_CE_dec; // doc says it is slow 
      Eigen::CompleteOrthogonalDecomposition<Matrix>  m_CE_dec; // available from Eigen 3.3.0, 40 us for decomposition, 40 us to get null space basis, 40 us to project Hessian
//      Eigen::JacobiSVD<Matrix, Eigen::HouseholderQRPreconditioner> m_CE_dec; // too slow
      Matrix m_ZT_H_Z;
      Matrix m_CI_Z;
#endif

      int m_neq;  /// number of equality constraints
      int m_nin;  /// number of inequality constraints
      int m_n;    /// number of variables
    };
  }
}

#endif // ifndef __invdyn_solvers_hqp_eiquadprog_hpp__