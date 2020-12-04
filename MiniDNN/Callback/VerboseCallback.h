#ifndef CALLBACK_VERBOSECALLBACK_H_
#define CALLBACK_VERBOSECALLBACK_H_

#include <Eigen/Core>
#include <iostream>
#include "../Config.h"
#include "../Callback.h"
#include "../Network.h"

namespace MiniDNN
{


///
/// \ingroup Callbacks
///
/// Callback function that prints the loss function value in each mini-batch training
///
class VerboseCallback: public Callback
{
    public:
        void post_training_batch(const Network* net, const Matrix& x, const Matrix& y)
        {
            const Scalar loss = net->get_output()->loss();
            std::cout << "[Epoch " << m_epoch_id << ", batch " << m_batch_id << "] Loss = "
                      << loss << std::endl;
        }

        void post_training_batch(const Network* net, const Matrix& x,
                                 const IntegerVector& y)
        {
            Scalar loss = net->get_output()->loss();
            std::cout << "[Epoch " << m_epoch_id << ", batch " << m_batch_id << "] Loss = "
                      << loss << std::endl;
        }
};


///
/// \ingroup Callbacks
///
/// Callback function that prints the loss function value in each mini-batch training
///
class CustomCallback: public Callback
{
    public:
        Scalar previous_loss = -1;
        void post_training_batch(const Network* net, const Matrix& x, const Matrix& y)
        {
            const Scalar loss = net->get_output()->loss();
            if (previous_loss == -1) {
                previous_loss = loss;
            }
            std::cout << loss << ", " << (previous_loss - loss) << std::endl;
            previous_loss = loss;
        }

        void post_training_batch(const Network* net, const Matrix& x, const IntegerVector& y)
        {
            const Scalar loss = net->get_output()->loss();
            if (previous_loss == -1) {
                previous_loss = loss;
            }
            std::cout << loss << ", " << (previous_loss - loss) << std::endl;
            previous_loss = loss;
        }
};


} // namespace MiniDNN


#endif /* CALLBACK_VERBOSECALLBACK_H_ */
