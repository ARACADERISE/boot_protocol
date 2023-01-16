<!doctype html>
<html>
<head>
</head>
<body>
<style>
.head1
{
    margin-left: 50px;
    background-color: #fceed1;
    text-align: center;
}
.head2
{
    margin-left: 25px;
    border-radius: 12px;
    margin-top: 40px;
    padding: 25px;
}
.head2_div
{
    margin-left: 25px;
}
.head3
{
    margin-left: 15px;
}
.this1
{
    background-color: #00DDFF;
    padding: 2px;
    border-radius: 12px;
}
.indent
{
    margin-left: 20px;
}
t
{
    color: #8076a3;
    font-weight: bolder;
}
t2
{
    color: #ff8928;
}
</style>

<h1 style="text-align: center;font-size: 42px;margin-left:25px;"><t>FAMP</t></h1>
<div class="head1">
<h1>Function And Macro based Protocol</h1>
</div>

<div class="head2">
<div class="head2_div">
<h1><b><u>Please Read:</u></b></h1>
<p style="font-size:16px">As of current, <t>FAMP</t> has to be installed <i>locally</i>. It is currently a work-in-progress to make it to where <t>FAMP</t> <i>does not</i> have to be installed locally.</br>It is also a work-in-progress for <t>FAMP</t>-related tools. This includes tools that initialize a <t>FAMP</t> project, reset projects, configure projects, run projects etc.</br>This documentation was created, and written, while beta version of <t>FAMP</t> were being written. Do not take this documentation with more than a grain of salt.</br></br>Alongside this, the protocol will not be deemed "done" until the protocol is capable of offering developers an API of which "libraries" can be written for the protocol. Read more on <t>FAMP</t>s api for writing libraries [link_to_page_here].</br></br>I assume you have gotten this PDF from my Github. If you find any possible errors(spelling mistakes, general mistakes, anything) then don't hesitate to tell me <b><u>@mocacdeveloper@gmail.com</u></b></p>
</div>
</div>

<div class="head3">
<h1 style="font-family:Helvetica;font-size:32px;"><t2>Introduction</h1><hr>
<div class="indent">
<p style="font-size:14px"><t>FAMP</t> stands for <b>F</b>unctional <b>A</b>nd <b>M</b>acro-based <b>P</b>rotocol. <t>FAMP</t> was inspired by the new uprising modern boot protocol <i>Limine</i>. <i>Limine</i> is based on a user-request/protocol-response type of system. <t>FAMP</t>, on the other hand, is based on a system where things happen <i>upon macro definition/function calling</i>. I decided I would take this approach because I liked the overall idea of "defining the protocol as the OS is written". The protocol depends on macro definitions so it knows what functionality it needs to have to suit your needs. It is rare to find yourself filling out a struct to accomplish tasks, however, I cannot promise you <i>won't ever</i> find yourself doing this.</br></br>Although the whole user-request/protocol-response system is quite clever, my decision to stick with a design of which the protocol depends on macro definitions and function calls has made the journey decently smooth. As for the pros of this particular design? That is a question that will most likely vary depending on the person.</br></br><t>FAMP</t> aims on being able to be used with any sort of skill level. The aim is to provide every last possible feature/function that can be used to piece together the OS. With this, the developer will have access to all functions that are used to get the OS to work. The developer can, then, decide to use the hundreds of functions, or, decide to use the "top-layer" functions that do all the "dirty work" for you.</br>For those who want to take control of the wheel a bit more will have the opportunity to become flexible as to how your bootloader code functions all and all. With this, developers who want to take control of the wheel will be capable of implementing <i>there own</i> functionality for the way things are done. The protocol further enhances the flexibility by supporting "handles". In <t>FAMP</t>, a "handle" is a protocol-related function that can be replaced by a user-defined function. Most, <i>if not all</i>, <t>FAMP</t> functions are "handles". Certain functions <i>might not be</i> a "handle" function due to the fact the operation of the function is critical.</br><b>SO</b>, not only are you given <i>all the functionality</i> you need, not only are you given functionality that does the "dirty work" for you, you are also capable of overriding the internal functionality of the protocol as you would like. Obviously you have to make sure you're accomplishing the intended purpose of the given function. But, nevertheless, the protocol <i>allows you</i> to redefine it. Odds are, however, you won't be seeing yourself writing your own functionality for the protocol. Instead, what you will be doing is writing an amazing OS using <t>FAMP</t></p>
</div>
</div>
</body>
</html>