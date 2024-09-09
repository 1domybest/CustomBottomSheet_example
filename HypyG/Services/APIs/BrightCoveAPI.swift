//
//  BrightCoveAPI.swift
//  HypyG
//
//  Created by 온석태 on 1/29/24.
//
import Alamofire

struct BrightCoveAPI {
    /// 로그인 API
    static func getStatus(request: BrightCoveGetStatusRequest, completion: @escaping (_ succeed: BrightCoveGetStatusResponse?, _ failed: Error?) -> Void) {
        
        let accessToken = KeychainServiceImpl.shared.accessToken
        
        APIService.shared.session.request(BrightCoveRouter.getStatus(request))
            .responseDecodable { (response: AFDataResponse<BrightCoveGetStatusResponse>) in
                switch response.result {
                case .success(let response):
                    completion(response, nil)
                case .failure(let error):
                    completion(nil, error)
                }
            }
    }
}
