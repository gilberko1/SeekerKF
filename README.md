# SeekerKF

Kalman filter for seeker head with 2 states :
1) angle
2) angle rate

angle is a measurement at 50hz and angle rate each 800 mili.
prediction stage is done on each angle measurement with linear state transition matrix and constant process noise.
update stage is done on angle rate measurement epoch with constant measurement noise.
posteriori covariance matrix update is done using Josef form.
