/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2022 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

//
// Created by 이상현 on 2022/11/07.
//


#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>

#include <iostream>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces should be used to prevent/limit name conflicts
    namespace examples {

        class Producer
        {
        public:
            void
            run()
            {
                m_face.setInterestFilter("/sch.ac.kr/calab/research.file",
                                         std::bind(&Producer::onInterest, this, _1, _2),
                                         nullptr, // RegisterPrefixSuccessCallback is optional
                                         std::bind(&Producer::onRegisterFailed, this, _1, _2));

                auto cert = m_keyChain.getPib().getDefaultIdentity().getDefaultKey().getDefaultCertificate();
                m_certServeHandle = m_face.setInterestFilter(security::extractIdentityFromCertName(cert.getName()),
                                                             [this, cert] (auto&&...) {
                                                                 m_face.put(cert);
                                                             },
                                                             std::bind(&Producer::onRegisterFailed, this, _1, _2));
                m_face.processEvents();
            }

        private:
            void
            onInterest(const InterestFilter&, const Interest& interest)
            {
                std::cout << ">> I: " << interest << std::endl;

                static const std::string content("FFD8FFE000104A46494600010100000100010000FFDB0043000C08090A09070C0A0A0A0D0D0C0E121E1312101012241A1B151E2B262D2C2A2629292F35443A2F32403329293B513C4046494C4D4C2E39545A534A59444B4C49FFDB0043010D0D0D12101223131323493129314949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949FFC000110800B300B303012200021101031101FFC4001B00010002030101000000000000000000000005060102040307FFC4003B100001030203050506050304030000000001000203041105122106314151611322718191324252A1B1C114152362D133E1F0244372A282B2F1FFC4001A010100020301000000000000000000000000030401020506FFC4002411000300020202020300030000000000000001020311042112310541132251233233FFDA000C03010002110311003F00B52222E81CD0888802222008888022220088B363C8FA218308888642222008888022220088880CA2220308888022220088880222200A1B17DA2A5C39CE8631DBD437DA60366B3FE478780D5786D3634FA5FF004348FB543C5E4907FB4D3CBF71F90D540609869C42AC3469130DC9DFE27AFF002A9F2394B1F4892637DB3B62ABC7B1890864EF823F820192C3A93A8F55DACD9DAA70CD2D73F37595EE3EB753F4F0454D088A26E568F9F52BD5717272F25BF64EB1A2BA6871AC3C67A6AB7CAD1EE171901FFC5DAFA15DD84E34CAD7FE1E76086AADA36FDD7F3CA7EC7552837F35178EE10CAA84D55282C9E3EF1CBA1B8E23A85371F9D70F54F68D6F12D744AA28FC1310388515E4B0A888E4940E2783874235F5520BBF34A96D159AD04445901111004444065111018444401111004444017857553286866AA9356C4C2EB73E43CCE8BDD5776D2A0B2869E981FEB4999DE0D17FA90B4C95E12E8CCADBD15492496691F2C8ECD3CCEBB8FEE3FC7D95EB00A36D1E18CB0B3A401C7C387F9D5532821EDF10821E67FB7DD7D1000D000D00D02F3BC9A7E8B50BB32888AA1285B46ECAEE877AD510100E6FE55B52CCBA4157FA6795CEEF477FECAC0A0F6AE33F9736A19EDC2FCC0FCC7CC05351C82589923773DA1C3CC5D77FE372796372FE8A795699B2222E891844440111101944440611110044440111100551DB275F14A46706C0E3EAEFECADCA9DB5EE0EC6E268F729C5FCDC7F855F95FF00266D8FD9CBB3AD0EC7E1BF017FAFF0AE55D59150D389640F717383191C62EF91C7735A39AA86CC34BB1D691EEB7EC55BA5A4136234B54E7E94ED780CB6F2E0066F2008F35E7F36BCFB2DC0A292AE5639D574D1D3EBDC6365ED0DBF768003E175D2BE7547B46FC47691EEC4F15ABC3F0F607968A5362DB7B22D637EBA156FD97C4E4C5B04654CBAC81EE8DCEB5B3D8FB56E1716F359CBC7A89F266FA259471C467A6A96C75F4A228647E48EA23933B2E4E81DA02D27CC5F8AAA6D6E3D5ADDA36E190D54B494D1B98247C46CE766B126FD01D14AE1B5B4F8D3F17C1A0AC9AAE8DB1010D54EDEF90E1637D05ECED41D16571DA8F3AF4344C63AC0FC1E7078007E6BD306717E0B44E3BCC0CFA2D712638E0F330BB338462EEE76B5CAD767DF9F01A3FDB1E43E2091F657BE29F748AD9C904445D820088880222203288880C22220088880222200BE7F8BD48ACC6EAE669BB33F66C3D1BA7D6EADF8F57FE5D84CB2B4FEB3FF004E2FF91E3E5A9F2542633BAD8997BBBBA0F4E2551E65F4A0931AFB2C5B2313582A2BA4D180687FCE83E6AC94B57154E6ECC905BBC1162B9B0EA064184B699C2C5C2EEB703FDB45E91521A38E69621DB4C5A4B5A6CD04EF03A5CAE1DD4D36CBF8D63FC4FCBFDBE881C4B61E82B6B9D511CF2D387BB33E36341049DF6BEE561C3A8A0C3A863A4A66648A31602F73D493CD41D2B715C2ABD8E9A1A7AE9F117E573C49D9BC3834BB202EBB728B1B0D14B4988CD4F1B9F5583E2B0B580B9CEFC3F68D0071CCC24596F5396E56BB46847E3FB2D498D4A27748F8270034C8C00E60375C15D381E0B47805148D84B9C5DDE9257EF75BC370E88CC744ED06930CC467CC2E0F6223691E2E2177C79EA68AD530185D236CE8F38716F98D168EAD4F8D3E8CAD6FBF469154C15AC921693AB4821C2DA1D1476CCC86315741268F864CE07476FF00FB03EAA46968594D23A4CE5CE22DBAD60A2B133F95E39062234864EE4DE0743E9A1F22ACF0B2CE3CDD7A66BCB986FF00C7E8B02222F4250088880222203288880C2222008888022287DA6C4CD061FD9C4EB54545D8C237B47BCEF2FA95AD5295B612DBD15DDA3C43F30C50B2375E0A6BB196DCE77BCEFB792F5D98A1FC5D776EF178A3D475FF00E9FA286632F9626697EE8E838957FC1A8C5161D1C796CE70CCEE9C87905C1E4E56F6FED96667E8EE45A4B232189D23CD9ADDE6DB970E2F8B47873031ADED6A1E3B91836F327805414B6F489D2DBD23AEAA9A9EAA031D5431CB16F2D91A08F1D5434949B2F192C71A26702D6CA403D0806CB7A0D9DC47682315788D56481DAB196D08E8DE5D4A86C42899415F3D231CC9044ECB99ADB02ADCE1A95BD924E3DF5B2DD4D534B3B6D4D3C3201C237836F20BDD500C5197071600E1B9C3423CC299C3B169E99A04D23EA6003BD985E48C7307DE1D37A8EB07F05626BD1665CB89520ADA192120171176DF9FF9A2F535117E14D4B5C1F164CE1CDD6E2D7D17A037008E22EA05B5D913FE115B375866A134B293DB5290C37DE5BEE9FB7929755DC40FE538F455C34826EECB6E44EBE86CEF55625E97899BF2E34FED14AE74C2222B46A11110194444061111004444015031DAB35B8D54480DE38CF631F8377FA9BABE4AFECA1924F81A5DE82EBE650DDD1B49DE45CF89D553E656A5492635F649E014C2A7156070BB5A403F53F657C552D8F60354F7FED79F980ADAB859DEEB4588357B5AF6398E176B8588E615726C15C71477E22AC470CA34A996E40B0B069E4A729E5789E4A699D77B7BCC75AD9DBFC8DDE8BA0804104020EF078AD22DC3249A6BB4414BB59881A26D342D861CAD0CED230731034D2FB9409249B937254E63F85454D136AA001AD71B39A370EA141AB8AFCD6CB38DA6BA0B2C7163839A6C46A085852380D3C3535C44C03831B9834F12B14F4B666ABC56CE8C159553C535388F2D148E0ECDF0FC4D6F43F2B9563468DCD161C02D6A648E9612F91D73B9AD6EA5C790EAA9D376F6556FBDB23F1FA76CF8549717C9DEF2DC7E456F80543AA70681CF377C60C4F3CCB4DBE965D354DED28A66916CD1BB4F2513B26F269EB23E0D983879B47F0BA5F177AB7257CCBEC9D4445DB2B8444406511101844440111101E554C3251CEC1BDD1B87A82BE6901FD28CFED1F45F500BE6D594E68EBEA6948FE948E03C378F910A8F357499263FB26F63DC05416F36BC7FDAEADAA8BB3D5029F116971B0CE2FE0E162AF4B879D7EC588227689F253D132B213692091A6F6F749B1FAAE78B691B93F5698E6FD8ED0FAA99AA819534D24120BB24696BBC0AA33E1969677D2CFF00D588D89F887070E856D8A6696993C4AA24713C5E5C41AD8833B389A6F96F724F55C0B00ACA9D4A95A44F32A56905963DF148248DC5AF6EA083A85845932D6FA648D3E3188CD554F4C26179640D2EC82E06F27D158E1A48E393B5739D2CBF1C86E47872F2503B314666A97620F1FA6D06387F77C4EFB7AAB32AB9749E9156D4EFA3C6ADC1947338F08DC7E4A1F64C1C95CEE1DA31BE8DFEEBBB1D9845863DB7D652183EFF0020BCB6622C9838948D6791D2F95EC3E415EF8C9FDDB2B6664B2222EE15C222203288880C222200888802AAED8D016C91E2518EE90229ADC3E177DBD15A9693451CF0BE19581F1C8D2D734EE20AD3242B972CCCBD3D9F3689FD94CD79F67D9778157EC26AC55D13493FA8CEEBFC79F9AA5E2F85CB8555762FBBE07DFB294FBC391FDC3E7BD7BE0789BE8AA4026E2D622FEDB7F90B83C8C2FD3F68B135AECBD28EC630A8B128DA737673C7EC4A05EDD0F30BB609A3A885B2C4E0E63B715E8A8A6E5ED13A7F68A255D355503AD570398DE12B46661F3E1E6BC5B2C6E176C8D23A382FA016DD701A7C2E798B3B1A3924E595A4AB139FFA89566FE94EED985C1AD76671DCD67789F20A530FC06A2ADC1F5AD7414FBFB3BF7DFE3F08F9AB3C34F1402D0C31C43F63437E8B596AA28A4EC807BE4B5F246DCC40EBC962B3B7D4A315959EB1C6D8A36B18D0D6B458340B00392DD73C75713E411B83E390EE6C8DCA4F8735C38DE2ACA385D146F02523BCEF807F2A152DBD10BA496C8EC6E67E258947434EEDEEECC11C0FBCEF20ACD146C86264518B318D0D68E4068143ECEE1AE8186B6A185B3CADB31877C6CFE4EF3E414D2F47C3C1F8A3BF6CA775B611115B35088880CA22203088880222200BC5D500121AD27C57B2D5CC63BDA68280E1AC6B2B699F4F3C6C7C6FDE08F9F42A9F88E0F53444B9A1F342350F6FB6CF11C7C42BD9A78CEEB8F35A9A569F78F9850E4C2B27B369AD149C271B968DFABDA5AEDF7F61FE3C8AB7516294B56000FC927C0F363E5CD7256ECD51D5B8BEDD9C877BD9A13E3C0F9A8A7ECA6214F7FC256C32B7E09416FF0023E8B999BE3E9F68967225E8B3D4C0E9F230BF2C5BDED1A17721E1CD6CEA6A7961EC5D1B433DD2D162C3C08E4AB0D8B68E9A32C14F35AD6BC52B5E3D09564C3E474B4319787095AD0D95AEF69AEB71FAAA193064C4B748966950A495EE6BE294832C472B8FC5C9DE63EEB79656425B986B23C30586A4A8EC5FF182AA3FCBC38CCE610F0D207741D0927A92B929F06C51F5515554D4C4C7C6ECCD05C6424F227416F05B62E2E4CABCA57461E453D13F2C71CF098651761DDCC1E6157A8F0CB57C9354C9DABA19086348D2FBF31E67929A35D0B05A60E8A4E3196926FD2DBC2D29E99EFED2692EC74AFCF92DAB46E00F5B0567818ABF2B54BD1A65A5AE8D85449CC1F10B715278B41F05B0A6671738AD84318F76FE2BB9D95FA368DE246DC5D6C8341608B20222203288880C2222008888022220088880222200BC65A68E57E739D8FB5B3C6E2D247236DEBD9162A552D52D8F479414F1C19B20399DED39C4971F1257AA2224A5690173CD11164044440111100444406511100444401111004444011110044440111100444401111004444011110044440111101FFFD9");

                // Create Data packet
                auto data = make_shared<Data>(interest.getName());
                data->setFreshnessPeriod(10_s);
                data->setContent(make_span(reinterpret_cast<const uint8_t*>(content.data()), content.size()));

                // in order for the consumer application to be able to validate the packet, you need to setup
                // the following keys:
                // 1. Generate example trust anchor
                //
                //         ndnsec key-gen /example
                //         ndnsec cert-dump -i /example > example-trust-anchor.cert
                //
                // 2. Create a key for the producer and sign it with the example trust anchor
                //
                //         ndnsec key-gen /example/testApp
                //         ndnsec sign-req /example/testApp | ndnsec cert-gen -s /example -i example | ndnsec cert-install -

                // Sign Data packet with default identity
                m_keyChain.sign(*data);
                // m_keyChain.sign(*data, signingByIdentity(<identityName>));
                // m_keyChain.sign(*data, signingByKey(<keyName>));
                // m_keyChain.sign(*data, signingByCertificate(<certName>));
                // m_keyChain.sign(*data, signingWithSha256());

                // Return Data packet to the requester
                std::cout << "<< D: " << *data << std::endl;
                m_face.put(*data);
            }

            void
            onRegisterFailed(const Name& prefix, const std::string& reason)
            {
                std::cerr << "ERROR: Failed to register prefix '" << prefix
                          << "' with the local forwarder (" << reason << ")" << std::endl;
                m_face.shutdown();
            }

        private:
            Face m_face;
            KeyChain m_keyChain;
            ScopedRegisteredPrefixHandle m_certServeHandle;
        };

    } // namespace examples
} // namespace ndn

int
main(int argc, char** argv)
{
    try {
        ndn::examples::Producer producer;
        producer.run();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}