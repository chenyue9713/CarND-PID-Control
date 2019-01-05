#include "PID.h"
#include<vector>
#include <numeric> 

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;

	this->p_error = 0.0;
	this->i_error = 0.0;
	this->d_error = 0.0;
}

void PID::UpdateParameters(double Kp, double Ki, double Kd) {
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
}

void PID::UpdateError(double cte) {
	d_error = cte - p_error;
	i_error += cte;
	p_error = cte;
	
}

double PID::TotalError() {
	return Kp * p_error + Ki * i_error + Kd * d_error;
}


void PID::Twiddle(double tol, double cte){
	std::vector<double> p;
	std::vector<double> dp;

	double d_Kp, d_Ki, d_Kd, sum_dp;
	d_Kp = 1.0;
	d_Ki = 1.0;
	d_Kd = 1.0;

	p.push_back(Kp);
	p.push_back(Ki);
	p.push_back(Kd);

	dp.push_back(d_Kp);
	dp.push_back(d_Ki);
	dp.push_back(d_Kd);

	double error, best_error;

	UpdateError(cte);
	best_error = TotalError();

	sum_dp = std::accumulate(dp.begin(),dp.end(),0);

	while(sum_dp > tol){
		
		for(unsigned int i = 0; i < p.size(); i++){
			p[i] += dp[i];
			UpdateParameters(p[0],p[1],p[2]);
			UpdateError(cte);
          	error = TotalError();

          	if (error < best_error){
          		best_error = error;
          		dp[i] *= 1.1;
          	}
          	else{
          		p[i] -= 2 * dp[i];
          		UpdateParameters(p[0],p[1],p[2]);
				UpdateError(cte);
          		error = TotalError();

          		if(error < best_error){
          			best_error = error;
          			dp[i] *= 1.1;
          		}
          		else{
          			p[i] += dp[i];
          			dp[i] *= 0.9;
          		}
          	}


		}
		sum_dp = std::accumulate(dp.begin(),dp.end(),0);

	}


} 

