using AccountServer.DB;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using SharedDB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace AccountServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class AccountController : ControllerBase
    {
        AppDbContext _context;
        SharedDbContext _sharedDbContext;

        // 정말 신기하게도 DI 를 통해서 여기에 context 와 sharedDbContext 가 들어간다.
        public AccountController(AppDbContext context, SharedDbContext sharedDbContext)
        {
            _context = context;
            _sharedDbContext = sharedDbContext;
        }

        [HttpPost]
        [Route("create")]
        // 놀랍게도 아래와 같이 [FromBody] 를 해주면, Post 요청 body 에서 ReqCreateAccount 를 추출해주고,
        // ResCreateAccount 를 리턴하면 클라이언트에 응답까지 ResCreateAccount 를 Body 에 넣어서 해준다.
        public ResCreateAccount CreateAccount([FromBody] ReqCreateAccount reqPacket)
        {
            ResCreateAccount resPacket = new ResCreateAccount();

            AccountDb account = _context.Accounts
                                .AsNoTracking()
                                .Where(account => account.AccountName == reqPacket.AccountName)
                                .FirstOrDefault();

            if (account == null)
            {
                _context.Accounts.Add(new AccountDb()
                {
                    AccountName = reqPacket.AccountName,
                    Password = reqPacket.Password
                });

                bool result = _context.SaveChangesEx();
                resPacket.result = result;
            }
            else
            {
                resPacket.result = false;
            }

            return resPacket;
        }

        [HttpPost]
        [Route("login")]
        public ResLoginAccount LoginAccount([FromBody] ReqLoginAccount reqPacket)
        {
            ResLoginAccount resPacket = new ResLoginAccount();

            AccountDb account = _context.Accounts
                                .AsNoTracking()
                                .Where(account => account.AccountName == reqPacket.AccountName && account.Password == reqPacket.Password)
                                .FirstOrDefault();

            if (account == null)
            {
                resPacket.result = false;
            }
            else
            {
                resPacket.result = true;

                // 토큰 발급
                DateTime expiredDatetime = DateTime.UtcNow;
                expiredDatetime = expiredDatetime.AddSeconds(600); // 토큰 발급 후 600초 후에 만료

                TokenDb tokenDb = _sharedDbContext.Tokens.Where(t => t.AcountDbId == account.AccountDbId).FirstOrDefault();

                // 토큰 있는 경우 토큰 재발급 및 갱신
                if (tokenDb != null)
                {
                    tokenDb.Token = new Random().Next(Int32.MinValue, Int32.MaxValue);
                    tokenDb.ExpiredDatetime = expiredDatetime;
                    _sharedDbContext.SaveChangesEx();
                }
                // 없는 경우 토큰 데이터 생성
                else
                {
                    tokenDb = new TokenDb()
                    {
                        AcountDbId = account.AccountDbId,
                        Token = new Random().Next(Int32.MinValue, Int32.MaxValue),
                        ExpiredDatetime = expiredDatetime
                    };

                    _sharedDbContext.Add(tokenDb); // 이 부분을 해줘야지 Db 상에서 해당 레코드가 추가되었음을 인식한다.
                    _sharedDbContext.SaveChangesEx();
                }

                resPacket.TokenDbId = tokenDb.TokenDbId;
                resPacket.Token = tokenDb.Token;
                resPacket.ServerList = new List<ServerInfo>();

                foreach(ServerDb serverDb in _sharedDbContext.Servers)
                {
                    resPacket.ServerList.Add(new ServerInfo()
                    {
                        Name = serverDb.Name,
                        IpAddress = serverDb.IpAddress,
                        Port = serverDb.Port,
                        CrowdedLevel = serverDb.CrowdedLevel
                    });
                }

            }

            return resPacket;
        }
    }
}
